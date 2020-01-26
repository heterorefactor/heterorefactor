import os
import argparse

# Instantiate the parser
parser = argparse.ArgumentParser(description='Transformation Arguments')
parser.add_argument('directory', type=str,
                    help='A required integer positional argument')

parser.add_argument('modified_directory', type=str,
                    help='A required integer positional argument')


parser.add_argument('guard_var', type=str,
                    help='A required integer positional argument')

parser.add_argument('invariant_file', type=str,
                    help='A required integer positional argument')


parser.add_argument('transformer_binary_dir', type=str,
                    help='A required integer positional argument')

parser.add_argument('size_of_instrumentation', type=str,
                    help='A required integer positional argument')

args = parser.parse_args()
directory = args.directory
mod_dir = args.modified_directory
transformer = args.transformer_binary_dir
guard_var = args.guard_var
invariant_file = args.invariant_file
size_of_instrumentation = args.size_of_instrumentation

for root, dirs, files in os.walk(directory):  
    for filename in files:
        if ".cpp" in filename or ".h" in filename:
            paramFileName = filename.split(".")[0]
            cmd = transformer+" "+ os.path.join(directory, filename)+" "+ os.path.join(mod_dir,filename) + " "+invariant_file + " "+ guard_var+" "+paramFileName+" "+size_of_instrumentation
            os.system(cmd)
