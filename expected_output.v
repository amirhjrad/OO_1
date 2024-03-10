module Behavioral(
    input A,
    input B,
    input C,
    input D,
    output W
);

    assign W = ~((A & B) | (C & D));
endmodule