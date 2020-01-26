set(LLVM_LINK_COMPONENTS 
support
)

add_clang_executable(transformer
  transformer.cpp
  ReadInvariants.cpp
  ReadGuardVars.cpp
  BitSavings.cpp
)
target_link_libraries(transformer
  PRIVATE
  clangTooling
  clangBasic
  clangASTMatchers
)
