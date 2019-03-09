# WrapPointer-Analyzer
    Log Example) // A, B, C, D -> address?
    new = { A = { new = { B } } }
    new = { C = { new = { D } } }
    assign = { B = C }
    assign = { D = A }
    delete = { A = { delete = { B } }
    delete = { C = { delete = { D } }
    
# Log -> Graph -> Analysis and Visualization.
    
