# **Lab 5: Flush+Reload Attack Against AES**

---

## **1. Overview**

This lab explores a widely-used **cache side-channel attack** — the **Flush+Reload attack** — to extract cryptographic keys from a victim AES binary. The lab focuses on benchmarking cache behavior, implementing the attack, and recovering AES key bytes both partially and fully.  

---

## **2. Objectives**

1. Benchmark and analyze **cache timing behavior** to differentiate between hits and misses.  
2. Implement the **Flush+Reload attack** to extract cryptographic keys from AES.  
3. Visualize and validate the results of cache benchmarks and key recovery.  
4. Explore **alternative attack strategies** to improve efficiency and compare results.  

--- 
## Results
1.Partial Key Recovery: Successfully recovered 4 AES key bytes (k2, k6, k10, k14) using lab5.py.
2.Full Key Recovery: Successfully recovered all 16 AES key bytes using Full_key.py by iterating through all T-tables.
3.Alternative Strategy: argmin.py provided faster results but was more susceptible to noise compared to the standard method.


## **3. Tools and Technologies Used**

### **Programming Languages**:  
- **C**: For cache benchmarking and profiling.  
- **Python**: For implementing the attack, key recovery, and visualization.  

### **Scripts and Files**:  
1. **`cache_benchmark.c`**: Measures cache hits and misses.  
2. **`lab5.py`**: Recovers 4 AES key bytes using the **Te0 table**.  
3. **`Full_key.py`**: Recovers the **full 16-byte AES last-round key** by iterating through all T-tables (`Te0`, `Te1`, `Te2`, `Te3`).  
4. **`argmin.py`**: Implements an **alternative attack strategy** focusing on cache misses with `np.argmin`.  

### **System Requirements**:  
- Linux-based system (x86 architecture).  
- AES victim binary for performing the attack.  

---

## **4. System Profiling**

To identify cache behavior, **cache timing benchmarks** were performed:

1. **Hits**:  
   - Access times were measured for data preloaded into the **L3 cache**.  
   - Results logged in **`hits_data.txt`**.  

2. **Misses**:  
   - Access times were measured after **flushing cache lines** to force access from main memory.  
   - Results logged in **`misses_data.txt`**.  

### **Cache Timing Threshold**:
- The threshold for distinguishing hits and misses was determined as **170.82 cycles**.  
- Access times:  
   - Below **170.82 cycles** → **Cache Hits**.  
   - Above **170.82 cycles** → **Cache Misses**.  

---

## **5. Attack Implementation**

### **Online Phase: Collect Cache Timing Samples**  
- Cache timing samples are collected during AES execution for specific cipher state columns.  
- **Observation**:  
   - Outliers occur in specific columns of the timing data, leaking information about key bytes.  

### **Offline Phase: Recover Key Bytes**  
The attack focuses on recovering AES key bytes using T-tables:

1. **Partial Key Recovery**:  
   - Implemented in **`lab5.py`**.  
   - Focuses on T-table **Te0** to recover 4 AES key bytes:  
     - **k2, k6, k10, k14**.  

2. **Full Key Recovery**:  
   - Implemented in **`Full_key.py`**.  
   - Recovers all 16 AES key bytes by iterating through all 4 T-tables (`Te0`, `Te1`, `Te2`, `Te3`).  

---

## **6. How to Run the Code**

### **Step 1: Benchmark Cache Hits and Misses**  
1. Compile the benchmark program:  
   ```bash
   gcc cache_benchmark.c -o cache_benchmark

## **7. Challenges Faced**

- **Timing Noise**:  
  - Cache timing data was occasionally noisy due to unrelated system processes.  
  - Accurate **threshold tuning** was critical to separate hits and misses.

- **Dataset Size**:  
  - Large datasets (cache hits and misses) required optimization for efficiency.  

- **Trade-offs**:  
  - **`argmax` Method** (Low Timings):  
    - ✅ More accurate  
    - ⏳ Slower processing  
  - **`argmin` Method** (High Timings):  
    - ⚡ Faster processing  
    - ❌ Less reliable due to noise  

---

## **8. Lessons Learned**

- **Cache Behavior**:  
  - Gained a deeper understanding of CPU cache hierarchies and timing behavior.

- **Side-Channel Attacks**:  
  - Learned how to use **Flush+Reload** to leak AES cryptographic keys.

- **Threshold Tuning**:  
  - Accurate threshold selection is crucial for reliable attack performance.

- **Efficiency vs. Accuracy**:  
  - Explored trade-offs between **speed** (argmin) and **accuracy** (argmax).

---

## **9. Conclusion**

- ✅ Successfully implemented the **Flush+Reload attack** to leak AES keys.  
- 🔑 Recovered both **partial** and **full AES last-round keys** using cache timing data.  
- 💡 Demonstrated the effectiveness of **cache-based side-channel attacks** for leaking cryptographic secrets.

---

