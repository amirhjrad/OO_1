module Testbench;

    // behavioral
    reg b_A;
    reg b_B;
    reg b_C;
    reg b_D;
    wire b_W;

    // structural
    reg s_A;
    reg s_B;
    reg s_C;
    reg s_D;
    wire s_W;

    Structural S_UUT(
        s_A,
        s_B,
        s_C,
        s_D,
        s_W
    );

    Behavioral B_UUT(
        b_A,
        b_B,
        b_C,
        b_D,
        b_W
    );

    initial begin
        repeat(10) begin
            #10
            b_A = $random();
            b_B = $random();
            b_C = $random();
            b_D = $random();
            s_A = b_A;
            s_B = b_B;
            s_C = b_C;
            s_D = b_D;
        end
        #20 $stop();
    end
    
endmodule