let diffusion = (nqubits) => {
    let prog = "\t// diffusion\n";
    // qc.h(range(n))
    for (i = 0; i < nqubits; i++) {
        prog += "\tq" + i + " := H[q" + i + "];\n";
    }
    
    // qc.x(range(n))
    for (i = 0; i < nqubits; i++) {
        prog += "\tq" + i + " := X[q" + i + "];\n";
    }
    // Apply multi-controlled Z gate
    prog += "\tq" + (nqubits - 1) + " := H[" + "q" + (nqubits - 1) + "];\n";
    let qubits = "";
    for (i = 0; i < nqubits; i++) {
        qubits += "q" + i + ((i == (nqubits - 1)) ? "" : ", ");
    }
    prog += "\t" + qubits + " := MCX[" + qubits + "];\n";
    prog += "\tq" + (nqubits - 1) + " := H[" + "q" + (nqubits - 1) + "];\n";

    // qc.x(range(n))
    for (i = 0; i < nqubits; i++) {
        prog += "\tq" + i + " := X[q" + i + "];\n";
    }
    // qc.h(range(n))
    for (i = 0; i <
         nqubits; i++) {
        prog += "\tq" + i + " := H[q" + i + "];\n";
    }
    return prog;
}

module.exports = {diffusion};