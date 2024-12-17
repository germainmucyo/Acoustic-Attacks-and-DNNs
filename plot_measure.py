import numpy as np
import matplotlib.pyplot as plt

hits = np.loadtxt("hits_data.txt")
misses = np.loadtxt("misses_data.txt")

threshold = (hits.mean() + misses.mean()) / 2


x_min = min(hits.min(), misses.min())
x_max = max(hits.max(), misses.max())


plt.figure(figsize=(14, 6))  
bins = range(int(x_min), int(x_max) + 1, 1)  
plt.hist(hits, bins=bins, alpha=0.7, label='Cache Hits', color='blue')
plt.hist(misses, bins=bins, alpha=0.7, label='Cache Misses', color='orange')


plt.axvline(threshold, color='green', linestyle='--', linewidth=1.5, label=f'Threshold: {threshold:.2f} cycles')


plt.xlim(0, 300)  
plt.ylim(0, 2000)  
plt.xticks(np.arange(0, 301, 50))  

plt.title("Cache Timing Distributions (Finalized)", fontsize=14)
plt.xlabel("Access Time (cycles)", fontsize=12)
plt.ylabel("Frequency", fontsize=12)
plt.legend(fontsize=10)


plt.grid(axis='both', linestyle=':', alpha=0.6)


plt.savefig("final_cache_timing_distribution.png", dpi=300)
plt.show()
