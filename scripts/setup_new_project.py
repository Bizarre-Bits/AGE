#!../venv/bin/python3

import sys
import os
import shutil
import re
from typing import TypedDict
from datetime import datetime


class CmdArgs(TypedDict):
    project_name: str
    project_namespace: str


def main():
    force = "--force" in sys.argv
    set_final_exe_bin = "--set-final-exe-bin" in sys.argv
    project_name = ""
    project_namespace = ""

    cmd_args: CmdArgs = parse_cmd_args()

    if cmd_args is None:
        project_name = get_project_name()
        project_namespace = get_project_namespace(project_name)
    else:
        project_name = cmd_args['project_name']
        project_namespace = cmd_args['project_namespace']

    cwd = os.getcwd()
    project_root = os.path.join(cwd, f"../{project_name}")
    project_root = os.path.normpath(project_root)

    prepare_dir(project_root, force)
    make_cmake_lists(project_root, project_name)

    generate_cpp_files(project_root, project_name, project_namespace)
    add_subdirectory(project_name)
    if set_final_exe_bin:
        setup_final_exe_bin(project_name)


def print_usage():
    print(f"Usage: {sys.argv[0]} --project [project_name] (--namespace [namespace])  (--force)")


def check_is_option(opt: str) -> bool:
    if opt.startswith("--"):
        return True
    return False


def is_valid_cpp_symbol_name(s: str) -> bool:
    return bool(re.match(r'^[a-zA-Z_][a-zA-Z_0-9]*$', s))


def parse_cmd_args() -> CmdArgs | None:
    args = sys.argv
    result: CmdArgs = {
        'project_name': "",
        'project_namespace': ""
    }
    if "--project" in args:
        try:
            result['project_name'] = args[args.index("--project") + 1]
            if check_is_option(result['project_name']):
                print(f"Expected project name, got: {result['project_name']}")
                print_usage()
                sys.exit(1)
            if not is_valid_cpp_symbol_name(result['project_name']):
                print(f"Incorrect project name: {result['project_name']}! Project name must also be a correct C++ symbol name.")
                sys.exit(1)
        except IndexError:
            print(f"There is no project name provided")
            print_usage()
            sys.exit(1)
    else:
        return None

    if "--namespace" in args:
        try:
            result['project_namespace'] = args[args.index("--namespace") + 1]
            if check_is_option(result['project_name']):
                print(f"Expected project namespace, got: {result['project_namespace']}")
                print_usage()
                sys.exit(1)
            if not is_valid_cpp_symbol_name(result['project_namespace']):
                print(f"Incorrect project namespace: {result['project_namespace']}! Project namespace must be a correct C++ symbol name.")
                sys.exit(1)
        except IndexError:
            print(f"There is no project namespace provided")
            print_usage()
            sys.exit(1)
    else:
        result['project_namespace'] = result['project_name']

    return result


def get_project_name() -> str:
    project_name = ""
    while project_name == "":
        project_name = input("Project Name: ")
        if not is_valid_cpp_symbol_name(project_name):
            print(f"Project name must also be a correct C++ symbol name.")
            project_name = ""
    return project_name


def get_project_namespace(project_name: str) -> str:
    project_namespace = ""
    while project_namespace == "":
        project_namespace = input(f"Project namespace [{project_name}]: ")
        if project_namespace == "":
            project_namespace = project_name
            break
        if is_valid_cpp_symbol_name(project_namespace):
            break
        print(f"Incorrect project namespace: {project_namespace}! Project namespace must be a correct C++ symbol name.")
        project_namespace = ""

    return project_namespace


def prepare_dir(project_root: str, force: bool):
    try:
        os.mkdir(project_root)
    except FileExistsError as err:
        if not force:
            print(f"Cannot create new project: {project_root} already exists")
            print(f"You can use --force to remove contents of {project_root} and create a new project in there")
            print(f"WARNING: this is not a safe action. Do it on your own risk")
            sys.exit(1)
        else:
            print(f"{project_root} exists but --force option was specified. Proceeding")
            shutil.rmtree(project_root)
            os.mkdir(project_root)


def make_cmake_lists(project_root: str, project_name: str):
    template_text: str = ""
    with open("./age_project_cmake_lists_template") as file:
        template_text = file.read()

    with open(f"{project_root}/CMakeLists.txt", "x") as file:
        file.write(template_text.format(project_name=project_name, PROJECT_NAME=project_name.upper()))


def generate_cpp_files(project_root: str, project_name: str, project_namespace: str):
    entry_point_template_text = ""
    new_layer_template_text = ""

    with open("./entry_point_cpp_template") as file:
        entry_point_template_text = file.read()

    os.mkdir(f"{project_root}/src")

    with open(f"{project_root}/src/{project_name}.cpp", "x") as file:
        file.write(entry_point_template_text.format(project_name=project_name,
                                                    project_namespace=project_namespace))


def setup_final_exe_bin(project_name: str):
    lines: list[str] = []
    with open("../CMakeLists.txt") as file:
        lines = file.readlines()

    with open("../CMakeLists.txt", "w") as file:
        is_dir_set = False
        for line in lines:
            if re.match(r"^set\(FINAL_EXE_BINARY_DIR [a-zA-Z_][a-zA-Z_0-9]*\)$", line.strip()):
                file.write(f"set(FINAL_EXE_BINARY_DIR {project_name})\n")
                is_dir_set = True
            elif not is_dir_set and line.strip() == "add_subdirectory(AGE)":
                file.write(f"set(FINAL_EXE_BINARY_DIR {project_name})\n")
                file.write(line)
            else:
                file.write(line)


def add_subdirectory(project_name: str):
    lines: list[str] = []
    with open("../CMakeLists.txt") as file:
        lines = file.readlines()
        if f"add_subdirectory({project_name})\n" in lines:
            return

    with open("../CMakeLists.txt", "w") as file:
        for line in lines:
            if line.strip() == "# Project Subdirectories":
                file.write(line)
                file.write(f"add_subdirectory({project_name})\n")
            else:
                file.write(line)


if __name__ == "__main__":
    main()
