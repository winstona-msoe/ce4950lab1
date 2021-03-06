# THIS FILE IS AUTOMATICALLY GENERATED
# Project: C:\2018 Winter\CE4950\GroupProject\ce4950lab1\NetworkingProject.cydsn\NetworkingProject.cyprj
# Date: Tue, 16 Jan 2018 20:00:21 GMT
#set_units -time ns
create_clock -name {CyILO} -period 10000 -waveform {0 5000} [list [get_pins {ClockBlock/ilo}] [get_pins {ClockBlock/clk_100k}] [get_pins {ClockBlock/clk_1k}] [get_pins {ClockBlock/clk_32k}]]
create_clock -name {CyIMO} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyPLL_OUT} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/pllout}]]
create_clock -name {CyMASTER_CLK} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/clk_sync}]]
create_generated_clock -name {CyBUS_CLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 2 3} [list [get_pins {ClockBlock/clk_bus_glb}]]
create_generated_clock -name {CLOCK2} -source [get_pins {ClockBlock/clk_sync}] -edges {1 241 481} [list [get_pins {ClockBlock/dclk_glb_0}]]
create_clock -name {CLOCK2(fixed-function)} -period 10000 -waveform {0 41.6666666666667} [get_pins {ClockBlock/dclk_glb_ff_0}]
create_generated_clock -name {CLOCK1} -source [get_pins {ClockBlock/clk_sync}] -edges {1 241 481} [list [get_pins {ClockBlock/dclk_glb_1}]]
create_generated_clock -name {CLOCK3} -source [get_pins {ClockBlock/clk_sync}] -edges {1 321 641} -nominal_period 13333.333333333332 [list [get_pins {ClockBlock/dclk_glb_2}]]

set_false_path -from [get_pins {__ONE__/q}]

# Component constraints for C:\2018 Winter\CE4950\GroupProject\ce4950lab1\NetworkingProject.cydsn\TopDesign\TopDesign.cysch
# Project: C:\2018 Winter\CE4950\GroupProject\ce4950lab1\NetworkingProject.cydsn\NetworkingProject.cyprj
# Date: Tue, 16 Jan 2018 20:00:17 GMT
