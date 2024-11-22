# Vijk represents a square at row i, col j, with value k.
def generate_rules():
    kb = [] 

    kb.append(f"# Every box needs a number")
    for i in range(1,10):
        for j in range(1, 10):
            clause = "(or " + " ".join(f"V{i}{j}{k}" for k in range(1, 10)) + ")"
            kb.append(clause)

    kb.append(f"# Every box only has one number")
    for i in range(1,10):
        for j in range(1, 10):
            for k in range(1, 10):
                clause = "(-> " + f"V{i}{j}{k} (and " + " ".join(f"(not V{i}{j}{l})" for l in range(1,10) if k != l) + "))"  
                kb.append(clause)

    kb.append(f"# Every row only has one of each number")
    for j in range(1,10):
        for i in range(1, 10):
            for k in range(1, 10):
                clause = "(-> " + f"V{i}{j}{k} (and " + " ".join(f"(not V{l}{j}{k})" for l in range(1,10) if i != l) + "))"  
                kb.append(clause)

    kb.append(f"# Every col only has one of each number")
    for i in range(1,10):
        for j in range(1, 10):
            for k in range(1, 10):
                clause = "(-> " + f"V{i}{j}{k} (and " + " ".join(f"(not V{i}{l}{k})" for l in range(1,10) if j != l) + "))"  
                kb.append(clause)

    kb.append(f"# Every 3x3 box only has one of each number")
    for i in range(1, 10):
        for j in range(1, 10):
            # Determine the top-left corner of the 3x3 subgrid this cell belongs to
            box_row_start = (i - 1) // 3 * 3 + 1
            box_col_start = (j - 1) // 3 * 3 + 1

            for k in range(1, 10):
                # Collect all cells in the same 3x3 subgrid as (i, j)
                subgrid_cells = [
                    f"V{p}{q}{k}"
                    for p in range(box_row_start, box_row_start + 3)
                    for q in range(box_col_start, box_col_start + 3)
                    if not (p == i and q == j)
                ]

                # Add the rule that if (i, j) contains k, no other cell in the subgrid can contain k
                clause = "(-> " + f"V{i}{j}{k} (and " + " ".join(f"(not {cell})" for cell in subgrid_cells) + "))"
                kb.append(clause)



    return kb

kb = generate_rules()
with open('sudoku.kb', 'w') as f:
    for c in kb:
        f.write(c+"\n")
