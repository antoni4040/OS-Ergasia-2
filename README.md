<h1>OS Project 2</h1>
<h3>by Antonios Karvelas, sdi1600060</h3>
<h4><i>Winter Semester 2019-2020</i></h4>

<h2>Heapsort and Quicksort:</h2>
<p>
For the two types of sorters I used the CLRS implementations (changed to the zero-index system, as CLRS starts arrays at index 1 just to mess with us I think). Instead of numeric comparisons I use a compare function that works differently for each sort field requested. For example, if we want to sort according to first names, it uses strcmp while for IDs it uses simple arithmetic etc. We want the compare function to return a negative if recordA < recordB, a positive if recordA > recordB and 0 if recordA = recordB. 
</p>

<h2>Getting the data:</h2>
<p>
To save some time and quite a lot of mallocs and unecessary callings to the kernel, I have a Record* and a Record** array. The first (which quite imaginatevely I call "vault") is where we put the actual data and Record** is for convenience, as swapping pointers during the sort is the right way to do things.
</p>

<h2>Signals:</h2>
<p>
I use sigaction for signals and I simply have a global array that counts the received signals. I increase it by 1 every time I catch a new signal.
</p>

<h2>Named pipes:</h2>
<p>
I use named pipes, which I create and then delete at the end of my program.
I create one named pipe for each sorter and then use select to listen. When the select is activated, I start receiving Records from the sorters into diffent arrays that I've allocated. After I read all of them, I simply merge them all together. <br>
There were discussions that reading and writing to pipes is better done in batches, but due to the fact that we don't actually have our data anywhere sequentially (we only have <strong>the pointers</strong> to the data in sorted order), reading/writing one-by-one is the right way to do it, I believe. 
</p>

<h2>Merging:</h2>
<p>
In order to merge n sorted arrays, simply merge the first one with the second one and then the result with the third one and so on and so forth. Nothing interesting here.
</p>

<h2>Makefile:</h2>
<p>
To compile the project, a simple <strong>make</strong> will do. We can also do <strong>make release</strong> for compilation with optimization level 3 enabled and <strong>make debug</strong> for compilation for debugging. <br>
<strong>make clean</strong> to gather our trash and <strong>make count</strong> to see how much code we've written(not that much to be honest) are also present.
</p>