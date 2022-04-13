General description or introduction of the problem and your solution:<br>
We are required to design a synthesizable version of Gaussian blur module. I designed a Gaussian blur module with correct functionality. And then I designed different versions of synthesizable Gaussian blur module by different stratus directives and compared the latencies and areas between them.<br>
<br>
Implementation details (data structure, flows and algorithms):<br>
From Lab 6, we practiced how synthesizable Sobel filter module is designed. I implemented Gaussian blur module by changing Sobel filter into Gaussian blur filter. By changing the algoritm from Sobel filter to Gaussian blur, we can get the Gaussian blur module. Also, the data width of data channels between testbench and Gaussian blur is needed to be change in order to get correct functionality.<br>
<br>
Additional features of your design and models:<br>
I added different stratus directives to do different hls strategies. I tried loop unrolling, pipelining, data path optimization and basic versions. The result of these strategies is shown in next part. Also, I added some codes in order to measure the total latency and average latency of different versions. Total latency is the latency needed to output whole Gaussian blurred picture and average latency is the latency needed to output one pixel of Gaussian blurred picture.<br>
<br>
Experimental results (on different benchmarks and settings):<br>
For basic version of hls:<br>
Area: 5824.3, Total Latency: 1441770, Average Latency: 22<br>
For data path optimization(DPA) version of hls:<br>
Area: 4285.8, Total Latency: 2031585, Average Latency: 31<br>
For pipeline version of hls with interval = 2:<br>
Area: 7360.2, Total Latency: 131070, Average Latency: 2<br>
For unrolling with DPA version of hls:<br>
Area: 7581.0, Total Latency: 1638375, Average Latency: 25<br>
<br>
Discussions and conclusions:<br>
Using DPA comparing to basic version, the area is smaller and the latency is a little larger. I think the reason may be that, for Gaussian blur module, I set hls latency constrain for for loop to 0~3 which is very tight. So, when DPA tries to optimize the module, it optimized its area instead of latency, however, the latency also grows. And the average latency of pipeline version is decreased to 2 since after pipelining, we can get input every 2 cycles and thus the average latency is 2. But the decrease of latency is exchanged by the increase of area in pipelining. And in unrolling with DPA version, the average latency compared to DPA version without unrolling is decreased. It is because unrolling makes the total latency drop but makes the area larger.<br>
<br>
For timing annotation of Gaussian Blur module, I added wait in Gaussian blur module in HW2 in order to make the simulation time of HW2 close to HW3. And the latency I added in HW2 is 20 clock period for each loop, the result leads to 12779520ns. By applying pipeline with interval = 2, the average latency may become 2 and thus I change the clock period waited by each loop of Gaussian blur module from 20 to 2. The result shows that the simulation time is 2686976ns.
