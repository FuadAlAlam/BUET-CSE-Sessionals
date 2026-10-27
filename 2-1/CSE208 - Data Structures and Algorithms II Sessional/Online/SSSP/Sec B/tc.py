import argparse
import random
import networkx as nx

"""
Test-case generator for "The Shattered Kingdom" problem.
Generates a single random test case (or multiple) following constraints:
- n cities, m directed edges
- k capitals, b blocked cities, e emergency posts
- No self-loops or parallel edges
- Edge weights in [-W, W]
- Blocked cities cannot coincide with capitals; emergency posts may overlap

Usage:
    python generate_tests.py --num 3 --n 100 --m 200 --k 5 --b 10 --e 5 --W 10
"""

def generate_single(n, m, k, b, e, W, seed=None):
    if seed is not None:
        random.seed(seed)

    # Ensure valid counts
    all_nodes = list(range(1, n + 1))
    capitals = random.sample(all_nodes, k)
    remaining = [x for x in all_nodes if x not in capitals]
    blocked = set(random.sample(remaining, b))
    # Emergency can include capitals or other nodes (but not blocked)
    eligible_for_emergency = [x for x in all_nodes if x not in blocked]
    emergency = random.sample(eligible_for_emergency, e)

    # Generate a random directed graph without self-loops or parallel edges
    G = nx.DiGraph()
    G.add_nodes_from(all_nodes)
    # Sample edges
    possible_edges = [(u, v) for u in all_nodes for v in all_nodes if u != v]
    edges = random.sample(possible_edges, min(m, len(possible_edges)))

    # Assign random weights
    weighted_edges = []
    for u, v in edges:
        w = random.randint(-W, W)
        weighted_edges.append((u, v, w))

    # Build ordering of output
    lines = []
    lines.append(f"{n} {len(weighted_edges)} {k} {b} {e}")
    lines.append(" ".join(map(str, capitals)))
    lines.append(" ".join(map(str, blocked)))
    lines.append(" ".join(map(str, emergency)))
    for u, v, w in weighted_edges:
        # Skip edges incident to blocked cities in the printed test
        # if u in blocked or v in blocked:
        #    continue
        lines.append(f"{u} {v} {w}")

    return "\n".join(lines)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Generate test cases for The Shattered Kingdom")
    parser.add_argument('--num', type=int, default=1, help='Number of test cases to generate')
    parser.add_argument('--n', type=int, default=5, help='Number of cities')
    parser.add_argument('--m', type=int, default=120, help='Number of edges')
    parser.add_argument('--k', type=int, default=11, help='Number of capitals')
    parser.add_argument('--b', type=int, default=4, help='Number of blocked cities')
    parser.add_argument('--e', type=int, default=12, help='Number of emergency posts')
    parser.add_argument('--W', type=int, default=15, help='Max absolute weight for edges')
    parser.add_argument('--seed', type=int, help='Random seed')

    args = parser.parse_args()

    for i in range(args.num):
        tc = generate_single(args.n, args.m, args.k, args.b, args.e, args.W, seed=(args.seed + i if args.seed is not None else None))
        print(tc)
        print('', end='', flush=True)
        if i != args.num - 1:
            print()  # separate testcases by blank line
