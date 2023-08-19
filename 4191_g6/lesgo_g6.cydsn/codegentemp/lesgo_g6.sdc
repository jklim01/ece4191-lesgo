# THIS FILE IS AUTOMATICALLY GENERATED
# Project: C:\Users\LJK\Desktop\Classes\ECE 4191\ece4191-lesgo\4191_g6\lesgo_g6.cydsn\lesgo_g6.cyprj
# Date: Sat, 19 Aug 2023 16:57:07 GMT
#set_units -time ns
create_clock -name {CyILO} -period 1000000 -waveform {0 500000} [list [get_pins {ClockBlock/ilo}] [get_pins {ClockBlock/clk_100k}] [get_pins {ClockBlock/clk_1k}] [get_pins {ClockBlock/clk_32k}]]
create_clock -name {CyIMO} -period 333.33333333333331 -waveform {0 166.666666666667} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyPLL_OUT} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/pllout}]]
create_clock -name {CyMASTER_CLK} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/clk_sync}]]
create_generated_clock -name {CyBUS_CLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 2 3} [list [get_pins {ClockBlock/clk_bus_glb}]]
create_generated_clock -name {motor_clk} -source [get_pins {ClockBlock/clk_sync}] -edges {1 25 49} [list [get_pins {ClockBlock/dclk_glb_0}]]
create_generated_clock -name {us_clk} -source [get_pins {ClockBlock/clk_sync}] -edges {1 25 49} [list [get_pins {ClockBlock/dclk_glb_1}]]
create_generated_clock -name {UART_1_IntClock} -source [get_pins {ClockBlock/clk_sync}] -edges {1 53 105} -nominal_period 2166.6666666666665 [list [get_pins {ClockBlock/dclk_glb_2}]]
create_generated_clock -name {color_sensor_clk} -source [get_pins {ClockBlock/clk_sync}] -edges {1 241 481} [list [get_pins {ClockBlock/dclk_glb_3}]]
create_generated_clock -name {ir_clk} -source [get_pins {ClockBlock/clk_sync}] -edges {1 300001 600001} [list [get_pins {ClockBlock/dclk_glb_4}]]

set_false_path -from [get_pins {__ONE__/q}]

# Component constraints for C:\Users\LJK\Desktop\Classes\ECE 4191\ece4191-lesgo\4191_g6\lesgo_g6.cydsn\TopDesign\TopDesign.cysch
# Project: C:\Users\LJK\Desktop\Classes\ECE 4191\ece4191-lesgo\4191_g6\lesgo_g6.cydsn\lesgo_g6.cyprj
# Date: Sat, 19 Aug 2023 16:56:54 GMT
