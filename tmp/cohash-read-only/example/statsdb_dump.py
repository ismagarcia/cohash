#!/usr/bin/python
# -*- coding: utf-8 -*-
import numpy as np
import pylab
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import matplotlib.cbook as cbook
import matplotlib.ticker as ticker

import sqlite3 as lite
import sys
import os

os.system("rm fir_float.sqlite3")

# Number of threads
num_threads = 2
# Number of runs of each fir_float_mode:
runs = 5
# Image data sizes:
#for width in [64, 128, 256, 512, 1024, 2048, 4096]:
#for width in [4096, 2048, 1024, 512, 256, 128, 64]:
#for width in xrange(64,4096,64):
for width in xrange(64,4096,512):
	height = width
	cmd = "/media/ARM_DEV/work/dev/experimental/cl/benchmarks/suites/neon/fir_float/fir_float --kernel-path=/media/ARM_DEV/work/dev/experimental/cl/benchmarks/suites/internal/image_filters/fir/fir_float/fir_kernels.cl --device=cpu --runs={} --width={} --height={} --num-threads={} --verify-result".format(runs, width, height, num_threads)
	os.system(cmd)

con = None
try:
	con  = lite.connect('fir_float.sqlite3')
	cur  = con.cursor()    

  # Get data by fir_float_mode
	cur.execute('SELECT * FROM fir_float_perf_bench WHERE fir_float_mode="OpenCL (single core)"')
	#cur.execute('SELECT * FROM fir_float_perf_bench WHERE fir_float_mode="OpenCL (single core)" AND (in_width>1500)')
	data = cur.fetchall()
	ocl_single_name         = [str(a[0]) for a in data]
	ocl_single_in_widths    = [str(a[1]) for a in data]
	ocl_single_avg_times_ns = [str(a[4]) for a in data]
	ocl_single_perf_rate    = [str(a[8]) for a in data]

	cur.execute('SELECT * FROM fir_float_perf_bench WHERE fir_float_mode="OpenCL (all cores)"')
	#cur.execute('SELECT * FROM fir_float_perf_bench WHERE fir_float_mode="OpenCL (all cores)" AND (in_width>1500)')
	data = cur.fetchall()
	ocl_all_name         = [str(a[0]) for a in data]
	ocl_all_in_widths    = [str(a[1]) for a in data]
	ocl_all_avg_times_ns = [str(a[4]) for a in data]
	ocl_all_perf_rate    = [str(a[8]) for a in data]

	cur.execute('SELECT * FROM fir_float_perf_bench WHERE fir_float_mode="C99 (Reference single core)"')
	#cur.execute('SELECT * FROM fir_float_perf_bench WHERE fir_float_mode="C99 (Reference single core)" AND (in_width>1500)')
	data = cur.fetchall()
	c99_single_name         = [str(a[0]) for a in data]
	c99_single_in_widths    = [str(a[1]) for a in data]
	c99_single_avg_times_ns = []#[str(a[4]) for a in data]
	c99_single_perf_rate    = []#[str(a[8]) for a in data]

	cur.execute('SELECT * FROM fir_float_perf_bench WHERE fir_float_mode="C99 (all cores)"')
	#cur.execute('SELECT * FROM fir_float_perf_bench WHERE fir_float_mode="C99 (all cores)" AND (in_width>1500)')
	data = cur.fetchall()
	c99_all_name         = [str(a[0]) for a in data]
	c99_all_in_widths    = [str(a[1]) for a in data]
	c99_all_avg_times_ns = []#[str(a[4]) for a in data]
	c99_all_perf_rate    = []#[str(a[8]) for a in data]

	cur.execute('SELECT * FROM fir_float_perf_bench WHERE fir_float_mode="C99+Neon Intrinsics (single core)"')
	#cur.execute('SELECT * FROM fir_float_perf_bench WHERE fir_float_mode="C99+Neon Intrinsics (single core)" AND (in_width>1500)')
	data = cur.fetchall()
	c99_neon_intrinsics_single_name         = [str(a[0]) for a in data]
	c99_neon_intrinsics_single_in_widths    = [str(a[1]) for a in data]
	c99_neon_intrinsics_single_avg_times_ns = []#[str(a[4]) for a in data]
	c99_neon_intrinsics_single_perf_rate    = []#[str(a[8]) for a in data]

	cur.execute('SELECT * FROM fir_float_perf_bench WHERE fir_float_mode="C99+Neon Intrinsics (all cores)"')
	#cur.execute('SELECT * FROM fir_float_perf_bench WHERE fir_float_mode="C99+Neon Intrinsics (all cores)" AND (in_width>1500)')
	data = cur.fetchall()
	c99_neon_intrinsics_all_name         = [str(a[0]) for a in data]
	c99_neon_intrinsics_all_in_widths    = [str(a[1]) for a in data]
	c99_neon_intrinsics_all_avg_times_ns = []#[str(a[4]) for a in data]
	c99_neon_intrinsics_all_perf_rate    = []#[str(a[8]) for a in data]

	cur.execute('SELECT * FROM fir_float_perf_bench WHERE fir_float_mode="Neon assembly (hand-edited single core)"')
	#cur.execute('SELECT * FROM fir_float_perf_bench WHERE fir_float_mode="Neon assembly (hand-edited single core)" AND (in_width>1500)')
	data = cur.fetchall()
	c99_neon_handcoded_single_name         = [str(a[0]) for a in data]
	c99_neon_handcoded_single_in_widths    = [str(a[1]) for a in data]
	c99_neon_handcoded_single_avg_times_ns = []#[str(a[4]) for a in data]
	c99_neon_handcoded_single_perf_rate    = []#[str(a[8]) for a in data]

	cur.execute('SELECT * FROM fir_float_perf_bench WHERE fir_float_mode="Neon assembly (hand-edited all cores)"')
	#cur.execute('SELECT * FROM fir_float_perf_bench WHERE fir_float_mode="Neon assembly (hand-edited all cores)" AND (in_width>1500)')
	data = cur.fetchall()
	c99_neon_handcoded_all_name         = [str(a[0]) for a in data]
	c99_neon_handcoded_all_in_widths    = [str(a[1]) for a in data]
	c99_neon_handcoded_all_avg_times_ns = []#[str(a[4]) for a in data]
	c99_neon_handcoded_all_perf_rate    = []#[str(a[8]) for a in data]

	#print ocl_single_avg_times_ns
	#print ocl_all_avg_times_ns
	#print c99_single_avg_times_ns 
	#print c99_all_avg_times_ns
	#print c99_neon_intrinsics_single_avg_times_ns
	#print c99_neon_intrinsics_all_avg_times_ns
	#print c99_neon_handcoded_single_avg_times_ns
	#print c99_neon_handcoded_all_perf_rate

	#print ocl_single_perf_rate
	#print ocl_all_perf_rate
	#print c99_single_perf_rate 
	#print c99_all_perf_rate
	#print c99_neon_intrinsics_single_perf_rate
	#print c99_neon_intrinsics_all_perf_rate
	#print c99_neon_handcoded_single_perf_rate
	#print c99_neon_handcoded_all_perf_rate

except lite.Error, e: 
	print "Error %s:" % e.args[0]
	sys.exit(1)    
finally:    
	if con:
		con.close()

# Plot 1) time vs image size
fig = plt.figure()
ax = fig.add_subplot(111)
ax.set_title('Performance time (ns)', {'fontweight' : 'bold'})
ax.plot(ocl_single_in_widths, ocl_single_avg_times_ns, 'o-') if (ocl_single_avg_times_ns != []) else ()
ax.plot(ocl_all_in_widths, ocl_all_avg_times_ns, 'o-')  if (ocl_all_avg_times_ns != []) else ()
ax.plot(c99_single_in_widths, c99_single_avg_times_ns, 'o-') if (c99_single_avg_times_ns != []) else ()
ax.plot(c99_all_in_widths, c99_all_avg_times_ns, 'o-') if (c99_all_avg_times_ns != []) else ()
ax.plot(c99_neon_intrinsics_single_in_widths, c99_neon_intrinsics_single_avg_times_ns, 'o-') if (c99_neon_intrinsics_single_avg_times_ns != []) else ()
ax.plot(c99_neon_intrinsics_all_in_widths, c99_neon_intrinsics_all_avg_times_ns, 'o-') if (c99_neon_intrinsics_all_avg_times_ns != []) else ()
ax.plot(c99_neon_handcoded_single_in_widths, c99_neon_handcoded_single_avg_times_ns, 'o-') if (c99_neon_handcoded_single_avg_times_ns != []) else ()
ax.plot(c99_neon_handcoded_all_in_widths, c99_neon_handcoded_all_avg_times_ns, 'o-') if (c99_neon_handcoded_all_avg_times_ns != []) else ()
# axis labels
log_enabled = 0
if (log_enabled):
	ax.set_yscale('log')
	pylab.ylabel(r'(log scale) Avg exec.time (ns)', {'color':'b','fontsize':14})
else:
	pylab.ylabel(r'Avg exec.time (ns)', {'color':'b','fontsize':14})
pylab.xlabel(r'Image size $(val)^{2}$', {'color':'b','fontsize':14})
# legend
box = ax.get_position()
ax.set_position([box.x0, box.y0 + box.height * 0.35, box.width, box.height * 0.65])
ocl_single_name = (ocl_single_name[0],)  if (ocl_single_avg_times_ns != []) else ()
ocl_all_name = (ocl_all_name[0],)  if (ocl_all_avg_times_ns != []) else () 
c99_single_name = (c99_single_name[0],)  if (c99_single_avg_times_ns != []) else ()
c99_all_name = (c99_all_name[0],)  if (c99_all_avg_times_ns != []) else ()  
c99_neon_intrinsics_single_name = (c99_neon_intrinsics_single_name[0],)  if (c99_neon_intrinsics_single_avg_times_ns != []) else ()
c99_neon_intrinsics_all_name = (c99_neon_intrinsics_all_name[0],)  if (c99_neon_intrinsics_all_avg_times_ns != []) else ()  
c99_neon_handcoded_single_name = (c99_neon_handcoded_single_name[0],)  if (c99_neon_handcoded_single_avg_times_ns != []) else ()
c99_neon_handcoded_all_name = (c99_neon_handcoded_all_name[0],)  if (c99_neon_handcoded_all_avg_times_ns != []) else ()  
fir_float_mode_names = ocl_single_name + ocl_all_name + c99_single_name + c99_all_name + c99_neon_intrinsics_single_name + c99_neon_intrinsics_all_name + c99_neon_handcoded_single_name+ c99_neon_handcoded_all_name
pylab.legend(fir_float_mode_names, 'upper center', bbox_to_anchor=(0.5, -0.15), shadow = True)
ltext = pylab.gca().get_legend().get_texts()
pylab.setp(ltext[0], fontsize = 12)


# Plot 2) perf_rate vs image size
fig = plt.figure()
ax = fig.add_subplot(111)
ax.set_title('Perf. % rate vs. Reference (fastest one)', {'fontweight' : 'bold'})
ax.plot(ocl_single_in_widths, ocl_single_perf_rate, 'o-') if (ocl_single_perf_rate != []) else ()
ax.plot(ocl_all_in_widths, ocl_all_perf_rate, 'o-')  if (ocl_all_perf_rate != []) else ()
ax.plot(c99_single_in_widths, c99_single_perf_rate, 'o-') if (c99_single_perf_rate != []) else ()
ax.plot(c99_all_in_widths, c99_all_perf_rate, 'o-') if (c99_all_perf_rate != []) else ()
ax.plot(c99_neon_intrinsics_single_in_widths, c99_neon_intrinsics_single_perf_rate, 'o-') if (c99_neon_intrinsics_single_perf_rate != []) else ()
ax.plot(c99_neon_intrinsics_all_in_widths, c99_neon_intrinsics_all_perf_rate, 'o-') if (c99_neon_intrinsics_all_perf_rate != []) else ()
ax.plot(c99_neon_handcoded_single_in_widths, c99_neon_handcoded_single_perf_rate, 'o-') if (c99_neon_handcoded_single_perf_rate != []) else ()
ax.plot(c99_neon_handcoded_all_in_widths, c99_neon_handcoded_all_perf_rate, 'o-') if (c99_neon_handcoded_all_perf_rate != []) else ()
# axis labels
pylab.ylabel(r'Perf. % rate', {'color':'b','fontsize':14})
pylab.xlabel(r'Image size $(val)^{2}$', {'color':'b','fontsize':14})
# legend
# Shink current axis's height by 10% on the bottom
box = ax.get_position()
ax.set_position([box.x0, box.y0 + box.height * 0.35, box.width, box.height * 0.65])
ocl_single_name = (ocl_single_name[0],)  if (ocl_single_perf_rate != []) else ()
ocl_all_name = (ocl_all_name[0],)  if (ocl_all_perf_rate != []) else () 
c99_single_name = (c99_single_name[0],)  if (c99_single_perf_rate != []) else ()
c99_all_name = (c99_all_name[0],)  if (c99_all_perf_rate != []) else ()  
c99_neon_intrinsics_single_name = (c99_neon_intrinsics_single_name[0],)  if (c99_neon_intrinsics_single_perf_rate != []) else ()
c99_neon_intrinsics_all_name = (c99_neon_intrinsics_all_name[0],)  if (c99_neon_intrinsics_all_perf_rate != []) else ()  
c99_neon_handcoded_single_name = (c99_neon_handcoded_single_name[0],)  if (c99_neon_handcoded_single_perf_rate != []) else ()
c99_neon_handcoded_all_name = (c99_neon_handcoded_all_name[0],)  if (c99_neon_handcoded_all_perf_rate != []) else ()  
fir_float_mode_names = ocl_single_name + ocl_all_name + c99_single_name + c99_all_name + c99_neon_intrinsics_single_name + c99_neon_intrinsics_all_name + c99_neon_handcoded_single_name + c99_neon_handcoded_all_name
pylab.legend(fir_float_mode_names,'upper center', bbox_to_anchor=(0.5, -0.15), shadow = True)
ltext = pylab.gca().get_legend().get_texts()
pylab.setp(ltext[0], fontsize = 12)


# show plot
plt.show()
