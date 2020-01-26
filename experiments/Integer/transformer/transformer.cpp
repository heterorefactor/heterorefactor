//------------------------------------------------------------------------------
// Clang rewriter sample. Demonstrates:
//
// * How to use RecursiveASTVisitor to find interesting AST nodes.
// * How to use the Rewriter API to rewrite the source code.
//
// Eli Bendersky (eliben@gmail.com)
// This code is in the public domain
//------------------------------------------------------------------------------
#include <cstdio>
#include <memory>
#include <sstream>
#include <string>

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Parse/ParseAST.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Rewrite/Frontend/Rewriters.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"
#include "ReadInvariants.h"
#include "ReadGuardVars.h"
#include "BitSavings.h"

using namespace clang;


std::string fileName;
BitSavings savings;
int instrumentedSize;
// By implementing RecursiveASTVisitor, we can specify which AST nodes
// we're interested in by overriding relevant methods.
class MyASTVisitor : public RecursiveASTVisitor<MyASTVisitor> {
public:
  MyASTVisitor(Rewriter &R) : TheRewriter(R) {}
  std::vector<std::string> v;
  int flags[2] = { 1, 0};
  bool temp = true; 
  bool guardInsert = true;
  std::string current_method = "";


  Expr *VisitBinaryOperator(BinaryOperator *E)
  {
    if(E->isCompoundAssignmentOp() || E->isAssignmentOp()){
      if (ArraySubscriptExpr *ASE = dyn_cast<ArraySubscriptExpr>(E->getLHS())) {
        ImplicitCastExpr *imp = dyn_cast<ImplicitCastExpr>(ASE->getBase());
        if(imp == NULL){
          std::cout<<"ImplicitCast is NULL\n";
          return E;
        }

        DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(imp->getSubExpr());

        if(DRE == NULL){
          std::cout<<"DeclRef is NULL\n";
        } else{
          std::string varName = DRE->getNameInfo().getName().getAsString();
          if (std::find(guardVars.begin(), guardVars.end(), varName) != guardVars.end())
          {
            // Element in vector.
            int size = invariantMap[current_method+":"+varName];
            std::stringstream SSBefore;
            
            ASTContext &ref = *ctx;
            SourceManager& mgr = ref.getSourceManager();
            SourceRange range = SourceRange(E->getRHS()->getSourceRange().getBegin(), E->getRHS()->getSourceRange().getEnd());
            StringRef s = Lexer::getSourceText(CharSourceRange::getTokenRange(range), mgr, ref.getLangOpts());
            std::string rhs = s;
            SSBefore <<";\nguard_"<<fileName<<"("<<rhs<<","<<size<<")";
            SourceLocation ST = E->getEndLoc();
            if(size>0){
              TheRewriter.InsertTextAfterToken(ST,SSBefore.str());
            }
          }
        }

      }  else{
      if (DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(E->getLHS())) {
          std::string varName = DRE->getNameInfo().getName().getAsString();
          int size = invariantMap[current_method+":"+varName];
          std::stringstream SSBefore;
          ASTContext &ref = *ctx;
          SourceManager& mgr = ref.getSourceManager();
          SourceRange range = SourceRange(E->getRHS()->getSourceRange().getBegin(), E->getRHS()->getSourceRange().getEnd());
          StringRef s = Lexer::getSourceText(CharSourceRange::getTokenRange(range), mgr, ref.getLangOpts());
          std::string rhs = s;
          SSBefore <<";\nguard_"<<fileName<<"("<<rhs<<","<<size<<")";
          SourceLocation ST = E->getEndLoc();
          if(size>0){
          TheRewriter.InsertTextAfterToken(ST,SSBefore.str());                  
          }
      }
    }    
    }     
    return E;
  }

  bool TraverseStmt(Stmt *s){
    RecursiveASTVisitor<MyASTVisitor>::TraverseStmt(s);
    return true;
  }

  bool VisitStmt(Stmt *s) {
    // Only care about If statements.
    if (isa<IfStmt>(s)) {
    }
    return true;
  }
   bool VisitVarDecl(VarDecl *s){
    QualType QT = s->getType();
    std::string TypeStr = QT.getAsString();   
    int replaceCount = 3; 
    if (TypeStr.find("unsigned") != std::string::npos) {
        replaceCount = 12;
    }
    std::string varName = s->getName(); 
    if(current_method.empty()){
      varName = varName;
    } else{
      varName= current_method+":"+varName;
    }

    int value;
    if(invariantMap.count(varName)){
      value = invariantMap[varName];
      SourceLocation typeLocation = s->getTypeSourceInfo()->getTypeLoc().getBeginLoc();
      SourceLocation typeLocationEnd = s->getTypeSourceInfo()->getTypeLoc().getEndLoc();    
      std::string modifiedType;
      if(value>32){
        modifiedType = "int";
      } else{
        modifiedType = typeMap[varName];
        if (std::find(guardVars.begin(), guardVars.end(), s->getName()) != guardVars.end()){
          //TODO Size of the array needs to be passed
          savings.addToModifiedMap(varName,value*instrumentedSize);
          savings.addToOriginalMap(varName,32*instrumentedSize);
        } else{
          savings.addToModifiedMap(varName,value);
          savings.addToOriginalMap(varName,32);
        }
      }

      const auto &SM = s->getASTContext().getSourceManager();
      TheRewriter.ReplaceText(s->getTypeSourceInfo()->getTypeLoc().getBeginLoc(),replaceCount,modifiedType);
      if(std::find(v.begin(), v.end(), s->getTypeSourceInfo()->getTypeLoc().getBeginLoc().printToString(SM)) == v.end()){
        v.push_back(s->getTypeSourceInfo()->getTypeLoc().getBeginLoc().printToString(SM));
      }
    }
    return true;
  }
  bool VisitFunctionDecl(FunctionDecl *f) {
    // Only function definitions (with bodies), not declarations.
    ctx = &f->getASTContext();
    SourceManager& sm = ctx->getSourceManager();
    DeclarationName DeclName = f->getNameInfo().getName();
    current_method = DeclName.getAsString();
    
    if(f->hasBody()){
      std::string bodyLocation = f->getBeginLoc().printToString(sm);
      if (bodyLocation.find("include") != std::string::npos) {
          std::cout << "found!" << '\n';
      } else{
        if(guardInsert){
          std::stringstream SSBefore;
          guardInsert = false;
          SSBefore << "bool guard_error_"<<fileName<<" = false;"
                    << "\n"
                <<"void guard_"<<fileName<<"(ap_int<65> value,int size,int sign){\n #pragma HLS inline off \n if(sign==1) {if(value<0){if(-1*value>(1<<(size-1)){guard_error_"<<fileName <<"= true;}} else{if(value>(1<<(size-1)){guard_error_"<<fileName <<"= true;}}} else{if(value>(1<<size)){guard_error_"<<fileName <<"= true;}\n}"<<"\n";
        SourceLocation ST = f->getSourceRange().getBegin();
          TheRewriter.InsertText(ST, SSBefore.str(), true, true);
        }
      }
    } else{
      if(flags[0]==1 && temp){
        temp = false;
        std::stringstream SSBefore;
        SSBefore <<"#include \"ap_int.h\""<<"\n";
        SourceLocation ST = f->getSourceRange().getBegin();
        TheRewriter.InsertText(ST, SSBefore.str(), true, true);
      }
    }
    return true;
  }

  void setInvariantMap(std::map<std::string, int> map,std::map<std::string, std::string> tMap){
    invariantMap = map;
    typeMap = tMap;
  }
  void setGuardVars(std::vector<std::string> vars){
    guardVars = vars;
  }
private:
  Rewriter &TheRewriter;
  std::map<std::string, int> invariantMap;
  std::map<std::string, std::string> typeMap;
  std::vector<std::string> guardVars;
  bool isFirstFunc = true;
  ASTContext *ctx;
};

// Implementation of the ASTConsumer interface for reading an AST produced
// by the Clang parser.
// Implementation of the ASTConsumer interface for reading an AST produced
// by the Clang parser.
class MyASTConsumer : public ASTConsumer {
public:
  MyASTConsumer(Rewriter &R, std::map<std::string, int> invariantMap, std::map<std::string, std::string> tMap, std::vector<std::string> guardVars) : Visitor(R) {
    Visitor.setInvariantMap(invariantMap,tMap);
    Visitor.setGuardVars(guardVars);
  }
  // Override the method that gets called for each parsed top-level
  // declaration.
  virtual bool HandleTopLevelDecl(DeclGroupRef DR) {
    for (DeclGroupRef::iterator b = DR.begin(), e = DR.end(); b != e; ++b)
      // Traverse the declaration using our AST visitor.
      Visitor.TraverseDecl(*b);
    return true;
  }

private:
  MyASTVisitor Visitor;
};

int main(int argc, char *argv[]) {
  if (argc < 2) {
    llvm::errs() << "Usage: ./transformer path_to_benchmark_source path_to_transformed_code path_to_<array_vars.txt> path_to_<invariants.txt> /bin/transformer <size of instrumentation>\n";
    return 1;
  }

  instrumentedSize = std::stoi(argv[6]);
  CompilerInstance TheCompInst;
  TheCompInst.createDiagnostics();

  LangOptions &lo = TheCompInst.getLangOpts();
  lo.CPlusPlus = 1;

  // Initialize target info with the default triple for our platform.
  auto TO = std::make_shared<clang::TargetOptions>();
  TO->Triple = llvm::sys::getDefaultTargetTriple();
  clang::TargetInfo *TI =
      TargetInfo::CreateTargetInfo(TheCompInst.getDiagnostics(), TO);
  TheCompInst.setTarget(TI);

  TheCompInst.createFileManager();
  FileManager &FileMgr = TheCompInst.getFileManager();
  TheCompInst.createSourceManager(FileMgr);
  SourceManager &SourceMgr = TheCompInst.getSourceManager();
  TheCompInst.createPreprocessor(TU_Module);
  TheCompInst.createASTContext();

  ReadGuardVars guardVars;
  guardVars.populateGuardInfo(argv[4]);
  guardVars.printInfo();
  //Read Invariants
  ReadInvariants invariants;
  invariants.populateInvariantInfo(argv[3]);
  // invariants.printMap();
  // A Rewriter helps us manage the code rewriting task.
  Rewriter TheRewriter;
  TheRewriter.setSourceMgr(SourceMgr, TheCompInst.getLangOpts());

  // Set the main file handled by the source manager to the input file.  
  fileName = argv[5];
  const FileEntry *FileIn = FileMgr.getFile(argv[1]);
  SourceMgr.setMainFileID(
      SourceMgr.createFileID(FileIn, SourceLocation(), SrcMgr::C_User));
  TheCompInst.getDiagnosticClient().BeginSourceFile(
      TheCompInst.getLangOpts(), &TheCompInst.getPreprocessor());

  // Create an AST consumer instance which is going to get called by
  // ParseAST.
  MyASTConsumer TheConsumer(TheRewriter,invariants.getInvariants(), invariants.getInvariantTypeMap(),guardVars.getGuards());

  // Parse the file to AST, registering our consumer as the AST consumer.
  ParseAST(TheCompInst.getPreprocessor(), &TheConsumer,
           TheCompInst.getASTContext());

  const RewriteBuffer *RewriteBuf =
      TheRewriter.getRewriteBufferFor(SourceMgr.getMainFileID());
  // llvm::outs() << std::string(RewriteBuf->begin(), RewriteBuf->end());
  std::error_code error_code;
  llvm::raw_fd_ostream outFile(argv[2], error_code, llvm::sys::fs::F_None);
  TheRewriter.getEditBuffer(SourceMgr.getMainFileID()).write(outFile);
  outFile.close();
  savings.printInfo();
  return 0;
}