#!/usr/bin/bash

show_help(){
    echo "Usage: bvcs <subcommand> [arguments...]"
    echo ""
    echo "Subcommands:"
    echo "  init                Initialize a new BVCS repository"
    echo "  add <file>...       Stage one or more files for the next commit"
    echo "  status              Show staged, modified, and untracked files"
    echo "  commit -m <msg>     Save a snapshot of all staged files"
    echo "  log                 Display the full commit history"
    echo "  diff [file]         Compare working copy to the latest commit"
    echo "  restore <file>      Restore a file from the latest commit"
    echo "  help                Print this help message"
}

check_repo(){
    if [ ! -d ".bvcs" ]; then
        echo "Error: Not a BVCS repository. Run 'init' first."
        exit 1
    fi
}


init_repo(){
    if [ -d ".bvcs" ]; then
        echo "Error: BVCS repository already exists."
        exit 1
    fi

    mkdir -p .bvcs/objects
    touch .bvcs/staging
    touch .bvcs/log
    touch .bvcs/HEAD

    echo "Initialized empty BVCS repository."
}


add_files(){
    if [ $# -eq 0 ]; then
        echo "Error: No files specified."
        exit 1
    fi

    for file in "$@"; do
        if [ ! -f "$file" ]; then
            echo "Error: '$file' not found."
            continue
        fi

        if grep -qx "$file" .bvcs/staging 2>/dev/null; then
            echo "Already staged: $file"
            continue
        fi

        echo "$file" >> .bvcs/staging
        echo "Staged: $file"
    done
}


show_status(){
    local has_output=false

    if [ -s .bvcs/staging ]; then
        echo "Staged for commit:"
        while IFS= read -r line; do
            echo "  $line"
        done < .bvcs/staging
        echo ""
        has_output=true
    fi

    local head
    head=$(cat .bvcs/HEAD 2>/dev/null)
    local modified=()

    if [ -n "$head" ] && [ -d ".bvcs/objects/$head/files" ]; then
        while IFS= read -r tracked_file; do
            local rel="${tracked_file#.bvcs/objects/$head/files/}"

            if grep -qx "$rel" .bvcs/staging 2>/dev/null; then
                continue
            fi

            if [ -f "$rel" ]; then
                if ! diff -q ".bvcs/objects/$head/files/$rel" "$rel" > /dev/null 2>&1; then
                    modified+=("$rel")
                fi
            fi
        done < <(find ".bvcs/objects/$head/files" -type f | sort)
    fi

    if [ ${#modified[@]} -gt 0 ]; then
        echo "Modified (not staged):"
        IFS=$'\n' sorted_modified=($(printf '%s\n' "${modified[@]}" | sort))
        unset IFS
        for f in "${sorted_modified[@]}"; do
            echo "  $f"
        done
        echo ""
        has_output=true
    fi

    local untracked=()

    while IFS= read -r wfile; do
        local rel="${wfile#./}"

        if grep -qx "$rel" .bvcs/staging 2>/dev/null; then
            continue
        fi

        if [ -n "$head" ] && [ -f ".bvcs/objects/$head/files/$rel" ]; then
            continue
        fi

        untracked+=("$rel")
    done < <(find . -type f ! -path './.bvcs/*' | sort)

    if [ ${#untracked[@]} -gt 0 ]; then
        echo "Untracked files:"
        IFS=$'\n' sorted_untracked=($(printf '%s\n' "${untracked[@]}" | sort))
        unset IFS
        for f in "${sorted_untracked[@]}"; do
            echo "  $f"
        done
        echo ""
        has_output=true
    fi

    if [ "$has_output" = false ]; then
        echo "Nothing to commit, working tree clean."
    fi
}


do_commit(){
    local message=""
    if [ "$1" = "-m" ] && [ -n "$2" ]; then
        message="$2"
    else
        echo "Error: Commit message required. Use -m \"message\"."
        exit 1
    fi

    if [ ! -s .bvcs/staging ]; then
        echo "Error: Nothing to commit."
        exit 1
    fi

    local num_commits
    num_commits=$(wc -l < .bvcs/log)
    local new_id
    new_id=$(printf '%04d' $((num_commits + 1)))

    mkdir -p ".bvcs/objects/$new_id/files"

    local head
    head=$(cat .bvcs/HEAD 2>/dev/null)
    if [ -n "$head" ] && [ -d ".bvcs/objects/$head/files" ]; then
        cp -r ".bvcs/objects/$head/files/." ".bvcs/objects/$new_id/files/"
    fi

    local file_count=0
    while IFS= read -r file; do
        local dest=".bvcs/objects/$new_id/files/$file"
        mkdir -p "$(dirname "$dest")"
        cp "$file" "$dest"
        file_count=$((file_count + 1))
    done < .bvcs/staging

    echo "$message" > ".bvcs/objects/$new_id/message"
    date +'%Y-%m-%d %H:%M:%S' > ".bvcs/objects/$new_id/timestamp"

    local timestamp
    timestamp=$(cat ".bvcs/objects/$new_id/timestamp")
    echo "$new_id|$timestamp|$message" >> .bvcs/log

    echo "$new_id" > .bvcs/HEAD

    > .bvcs/staging

    echo "[$new_id] $message"
    echo "$file_count file(s) committed."
}


show_log(){
    if [ ! -s .bvcs/log ]; then
        echo "No commits yet."
        return
    fi

    tac .bvcs/log | while IFS='|' read -r id ts msg; do
        echo "commit $id"
        echo "Date:   $ts"
        echo "Message: $msg"
        echo ""
    done
}


show_diff(){
    local head
    head=$(cat .bvcs/HEAD 2>/dev/null)

    if [ -z "$head" ]; then
        echo "Error: No commits yet."
        exit 1
    fi

    if [ $# -gt 0 ]; then
        local file="$1"
        if [ ! -f ".bvcs/objects/$head/files/$file" ]; then
            echo "Error: '$file' is not tracked."
            exit 1
        fi

        if diff -q ".bvcs/objects/$head/files/$file" "$file" > /dev/null 2>&1; then
            echo "$file: no changes."
        else
            diff -u --label ".bvcs/objects/$head/files/$file" --label "$file" \
                ".bvcs/objects/$head/files/$file" "$file"
        fi
    else
        find ".bvcs/objects/$head/files" -type f | sort | while IFS= read -r tracked; do
            local rel="${tracked#.bvcs/objects/$head/files/}"

            if diff -q ".bvcs/objects/$head/files/$rel" "$rel" > /dev/null 2>&1; then
                echo "$rel: no changes."
            else
                diff -u --label ".bvcs/objects/$head/files/$rel" --label "$rel" \
                    ".bvcs/objects/$head/files/$rel" "$rel"
            fi
        done
    fi
}


restore_file(){
    if [ $# -eq 0 ]; then
        echo "Error: No file specified."
        exit 1
    fi

    local head
    head=$(cat .bvcs/HEAD 2>/dev/null)

    if [ -z "$head" ]; then
        echo "Error: No commits yet."
        exit 1
    fi

    local file="$1"

    if [ ! -f ".bvcs/objects/$head/files/$file" ]; then
        echo "Error: '$file' not found in commit $head."
        exit 1
    fi

    read -rp "Restore '$file' from commit $head? [y/N]: " answer
    if [ "$answer" = "y" ] || [ "$answer" = "Y" ]; then
        mkdir -p "$(dirname "$file")"
        cp ".bvcs/objects/$head/files/$file" "$file"
        echo "Restored: $file"
    else
        echo "Aborted."
    fi
}


subcmd="${1:-}"
shift 2>/dev/null

case "$subcmd" in
    init)
        init_repo
        ;;
    help)
        show_help
        ;;
    add)
        check_repo
        add_files "$@"
        ;;
    status)
        check_repo
        show_status
        ;;
    commit)
        check_repo
        do_commit "$@"
        ;;
    log)
        check_repo
        show_log
        ;;
    diff)
        check_repo
        show_diff "$@"
        ;;
    restore)
        check_repo
        restore_file "$@"
        ;;
    "")
        show_help
        ;;
    *)
        echo "Error: Unknown subcommand '$subcmd'."
        exit 1
        ;;
esac
