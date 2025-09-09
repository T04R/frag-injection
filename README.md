# frag-injection

<div align="center">
  <h1></h1>
  <img src="diagrams/diagram0.svg" width="90%" /><br />
</div>

#

### Core Concept & Philosophy

The fundamental idea behind this project is to deconstruct a classic process injection technique into discrete, isolated segments. Each segment operates independently within its own process space and communicates through shared memory pointers rather than a linear, monolithic execution flow. By fragmenting the injection lifecycle, we prevent EDR (Endpoint Detection and Response) systems and AV (Antivirus) solutions from observing the entire malicious sequence from a single process. Instead, they are presented with individual, seemingly benign operations performed by separate, unrelated processes. This significantly reduces the likelihood of detection based on behavioral analysis.

### Technical Deep Dive: Deconstructing Classic Injection

A standard, well-known process injection technique (e.g., **`VirtualAllocEx`** / **`WriteProcessMemory`** / **`CreateRemoteThread`**) is split into three separate components:

**1. Injector Process** `(injector.exe)`

> This process is responsible for the initial setup within the target process.

- `Find PID:` It first obtains the Process ID (PID) of the target process (e.g., via CreateToolhelp32Snapshot).
- `OpenProcess:` It uses the acquired PID to call OpenProcess, requesting specific permissions (e.g., PROCESS_VM_OPERATION, PROCESS_QUERY_INFORMATION) to interact with the target process's memory.
- `VirtualAllocEx:` It allocates a region of memory inside the target process's address space using VirtualAllocEx. This region will later hold the shellcode.
- `Output:` The critical output of this process is the Target PID and the memory address returned by VirtualAllocEx. This data must be passed to the next segments (e.g., written to a file, sent via IPC, passed as a command-line argument to the next process).

**2. Writer Process** `(writer.exe)`

> This process is solely dedicated to writing the payload into the pre-allocated memory region.

- `Input:` It takes the Target PID and the memory address from the previous segment.
- `OpenProcess:` It calls OpenProcess again, this time requesting write permissions (e.g., PROCESS_VM_WRITE) on the target process using the provided PID.
- `WriteProcessMemory:` It writes the shellcode byte-by-byte into the exact memory address within the target process that was allocated by the injector.exe.

**3. Executer Process** `(executer.exe)`

> This process triggers the execution of the injected shellcode.

- `Iput:` It takes the same Target PID and memory address.
- `OpenProcess:` It calls OpenProcess one final time, requesting execution permissions (e.g., PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_CREATE_THREAD).
- `CreateRemoteThread:` It creates a new thread in the remote (target) process that starts execution at the beginning of the allocated memory address, effectively running the shellcode.


<div align="center">
  <h1></h1>
  <img src="diagrams/diagram1.svg" width="90%" /><br />
</div>

---

### Why This Method is Effective Against EDR/AV

1. Behavioral Segmentation: Modern EDRs rely heavily on correlating events within a single process to detect malicious chains. This technique breaks that chain. The EDR sees:
   · Process A (e.g., a benign-looking tool) allocating memory in another process. This is a common legitimate operation.
   · Process B (another seemingly benign tool) writing to a memory address in a remote process.
   · Process C (yet another tool) starting a thread in a remote process.
   · Without strong cross-process correlation, each event appears harmless in isolation, making it extremely difficult for the EDR to flag the entire sequence as malicious.
2. Reduced Suspicion: Each process can be designed to perform a single, specific task. The injector could be disguised as a memory profiling tool, the writer as a debugger, and the executer as a process interaction utility. This "separation of duties" appears much less suspicious than a single process performing all steps of a known injection pattern.
3. Obscuring the Full Picture: By splitting the technique, the complete "injection story" is never visible from the perspective of any single process. This directly circumvents detection logic that looks for the entire sequence of VirtualAllocEx -> WriteProcessMemory -> CreateRemoteThread happening in quick succession from the same source.

**Expansion and Methodology**

This pattern is not limited to classic injection. The same principle can be applied to virtually any other code execution technique (e.g., Process Hollowing, AtomBombing, Module Stomping, APC Queue Injection).

- The Key: The core of this obfuscation method is to identify the critical steps of a technique and isolate them into separate binaries or execution contexts.
- Inter-Process Communication (IPC): The segments must communicate. This can be achieved through various stealthy methods:
  - Command-line Arguments: Passing the PID and memory address to the next process.
  - Named Pipes / Sockets: For more advanced communication.
  - Intermediate Files: Storing the critical data in a temporary file.
  - Windows Registry: Storing the values in a registry key.
  - Shared Memory Sections: Using file mapping objects for direct memory sharing.

#

The goal is to create a system where processes work together by referencing each other's memory addresses and coordinating execution, all while maintaining operational separation to evade defensive software.



