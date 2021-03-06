
?
HLS_Final_Project-Default$07c69333-f5a4-4211-9388-9b1d64d35ca6+Vitis IDE session HLS_Final_Project-Default*e"a/home/r09922017/workspace/HLS_Final_Project/Emulation-HW/HLS_Final_Project-Default/.guidance.html2c"_/home/r09922017/workspace/HLS_Final_Project/Emulation-HW/HLS_Final_Project-Default/.guidance.pb *?
?b?bPLRAM_USAGE"?
?
here? :	text/htmlJ?<html>PLRAMs can increase your system performance by providing low-latency, high-bandwidth access to your data. For detailed explanation and recommendation: <a href="https://www.xilinx.com/cgi-bin/docs/rdoc?t=vitis+guidance;v=2019.2;d=PLRAM_USAGE.html">PLRAM_USAGE</a></html>:0B> 0J'
PLRAMs were used by 0 port(s).
20R?
6Utilize PLRAMs to maximize performance. Click !URI%1!.?
??
here? :	text/htmlJ?<html>PLRAMs can increase your system performance by providing low-latency, high-bandwidth access to your data. For detailed explanation and recommendation: <a href="https://www.xilinx.com/cgi-bin/docs/rdoc?t=vitis+guidance;v=2019.2;d=PLRAM_USAGE.html">PLRAM_USAGE</a></html>Z Kernel Data Transfer
?c?cTRACE_MEMORY"?
?
here? :	text/htmlJ?<html>Users can specify the trace buffer memory type for profiling. The default is FIFO:8K, which may not be large enough to hold all trace events on a given device. For detailed explanation and recommendation: <a href="https://www.xilinx.com/cgi-bin/docs/rdoc?t=vitis+guidance;v=2019.2;d=TRACE_MEMORY.html">TRACE_MEMORY</a></html>:FIFOB	!= FIFOJ6
*Trace buffer memory was specified as FIFO.
2FIFOR?
BSpecify memory type to maximize trace buffer depth. Click !URI%1!.?
??
here? :	text/htmlJ?<html>Users can specify the trace buffer memory type for profiling. The default is FIFO:8K, which may not be large enough to hold all trace events on a given device. For detailed explanation and recommendation: <a href="https://www.xilinx.com/cgi-bin/docs/rdoc?t=vitis+guidance;v=2019.2;d=TRACE_MEMORY.html">TRACE_MEMORY</a></html>Z Resource Usage:?
?d?dOBJECTS_RELEASED?
?
here? :	text/htmlJ?<html>Profiling may contain incomplete information. Please ensure all OpenCL objects are released by your host code (e.g., clReleaseProgram()). For detailed explanation and recommendation: <a href="https://www.xilinx.com/cgi-bin/docs/rdoc?t=vitis+guidance;v=2019.2;d=OBJECTS_RELEASED.html">OBJECTS_RELEASED</a></html>2true:trueB?
3Check for OpenCL objects released by the host code.
2trueJE
CEnsure all OpenCL objects are released by host code. Click !URI%1!.R Resource UsageB?	
?
PLRAM_USAGE?
PLRAM_USAGEPLRAM Usageschuey"!PLRAMs were used by %STR port(s).: Kernel Data TransferB6Utilize PLRAMs to maximize performance. Click !URI%1!.J?
?
here? :	text/htmlJ?<html>PLRAMs can increase your system performance by providing low-latency, high-bandwidth access to your data. For detailed explanation and recommendation: <a href="https://www.xilinx.com/cgi-bin/docs/rdoc?t=vitis+guidance;v=2019.2;d=PLRAM_USAGE.html">PLRAM_USAGE</a></html>Pz> 0?p :	text/htmlJa<html>This rule checks whether PLRAMs (e.g., BlockRAMs, URAMs) are utilized on the device.</html>
?
TRACE_MEMORY?
TRACE_MEMORYTrace Memoryschuey"*Trace buffer memory was specified as %STR.: Resource UsageBBSpecify memory type to maximize trace buffer depth. Click !URI%1!.J?
?
here? :	text/htmlJ?<html>Users can specify the trace buffer memory type for profiling. The default is FIFO:8K, which may not be large enough to hold all trace events on a given device. For detailed explanation and recommendation: <a href="https://www.xilinx.com/cgi-bin/docs/rdoc?t=vitis+guidance;v=2019.2;d=TRACE_MEMORY.html">TRACE_MEMORY</a></html>Pz	!= FIFO?f :	text/htmlJW<html>This rule checks whether a memory type was specified for the trace buffer.</html>J?
?
OBJECTS_RELEASED?
OBJECTS_RELEASEDObjects Releasedschuey"3Check for OpenCL objects released by the host code.: Resource UsageBCEnsure all OpenCL objects are released by host code. Click !URI%1!.J?
?
here? :	text/htmlJ?<html>Profiling may contain incomplete information. Please ensure all OpenCL objects are released by your host code (e.g., clReleaseProgram()). For detailed explanation and recommendation: <a href="https://www.xilinx.com/cgi-bin/docs/rdoc?t=vitis+guidance;v=2019.2;d=OBJECTS_RELEASED.html">OBJECTS_RELEASED</a></html>Pztrue?f :	text/htmlJW<html>This rule checks if OpenCL objects are properly released by the host code.</html>