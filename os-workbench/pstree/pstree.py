import psutil

def print_process_tree(parent_pid, indent=""):
    try:
        parent = psutil.Process(parent_pid)
        print(f"{indent}├─ {parent.name()} [{parent.pid}]")
        children = parent.children(recursive=False)
        count = len(children)
        for i, child in enumerate(children):
            if i == count - 1:
                print_process_tree(child.pid, indent + "    ")
            else:
                print_process_tree(child.pid, indent + "│   ")
    except psutil.NoSuchProcess:
        return

def main():
    # get the PID of the root process (usually 0 or 1)
    root_pid = psutil.Process().ppid()
    print_process_tree(root_pid)

if __name__ == "__main__":
    main()
