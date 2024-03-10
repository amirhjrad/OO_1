module Structural(
    input A,
    input B,
    input C,
    input D,
    output W
);

    wire w1;
    wire w2;
    wire w3;

    and i1(w1, A, B);
    and i2(w2, C, D);
    or i3(w3, w1, w2);
    not i4(W, w3);
    
endmodule