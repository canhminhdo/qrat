let diffusion = (nqubits) => {
    let prog = "\t// diffusion\n";
    // qc.h(range(n))
    for (i = 0; i < nqubits; i++) {
        prog += "\tH[q" + i + "];\n";
    }
    
    // qc.x(range(n))
    for (i = 0; i < nqubits; i++) {
        prog += "\tX[q" + i + "];\n";
    }
    // Apply multi-controlled Z gate (MCZ = H MCX H)
    // prog += "\tH[" + "q" + (nqubits - 1) + "];\n";
    let qubits = "";
    for (i = 0; i < nqubits; i++) {
        qubits += "q" + i + ((i == (nqubits - 1)) ? "" : ", ");
    }
    prog += "\tMCZ[" + qubits + "];\n";
    // prog += "\tMCX[" + qubits + "];\n";
    // prog += "\tH[" + "q" + (nqubits - 1) + "];\n";

    // qc.x(range(n))
    for (i = 0; i < nqubits; i++) {
        prog += "\tX[q" + i + "];\n";
    }
    // qc.h(range(n))
    for (i = 0; i <
         nqubits; i++) {
        prog += "\tH[q" + i + "];\n";
    }
    return prog;
}

module.exports = {diffusion};