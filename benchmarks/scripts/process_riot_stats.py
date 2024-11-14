import csv
import sys

num_inputs = 0
first_input_ts = 0
first_output_ts = 0
last_input_ts = 0
last_output_ts = 0

ip_throughputs = []
op_throughputs = []
avg_latency = []

window_size = 10
ip_history = []
op_history = []
latency_history = []

with open("./riot_sink.csv") as sink_file:
    csvreader = csv.reader(sink_file)
    first_line = next(csvreader)
    first_input_ts = int(first_line[1])
    first_output_ts = int(first_line[2])
    num_inputs += 1

    for row in csvreader:
        ip_history.append(int(row[1]))
        op_history.append(int(row[2]))
        latency_history.append(int(row[2]) - int(row[1]))
        last_input_ts = int(row[1])
        last_output_ts = int(row[2])
        num_inputs += 1
        if len(ip_history) == window_size:
            avg_latency.append(sum(latency_history)/window_size)
            latency_history = []
            ip_throughputs.append((window_size * 1000) / (ip_history[window_size - 1] - ip_history[0]))
            ip_history = []
            op_throughputs.append((window_size * 1000) / (op_history[window_size - 1] - op_history[0]))
            op_history = []

with open("./riot_results.txt", "w+") as file:
    print("===== OVERALL STATS =====")
    print(f"Avg latency: {sum(avg_latency) / len(avg_latency)}")
    avg_ip_rate = (num_inputs * 1000) / (last_input_ts - first_input_ts)
    print(f"Avg o/p throughput: {((num_inputs * 1000) / (last_output_ts - first_output_ts)):.2f} messages/s")
    print("")
    print("Windowed stats will be written to riot_results.txt...")
    file.write("Avg Latency, Output Throughput, Jitter\n")
    for i in range(0, len(op_throughputs)):
        jitter = (op_throughputs[i] - ip_throughputs[i]) / avg_ip_rate
        file.write(f"{avg_latency[i]:.2f}, {op_throughputs[i]:.3f}, {jitter:.3f}\n")