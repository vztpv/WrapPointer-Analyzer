# WrapPointer-Analyzer
    Log Example) // A, B, C, D -> address?
    new = { A = { make = { B } } }
    new = { C = { make = { D } } }
    assign = { B = C }
    assign = { D = A }
    delete = { A = { delete = { B } }
    delete = { C = { delete = { D } }
    
# Log -> Graph -> Analysis and Visualization.
    
