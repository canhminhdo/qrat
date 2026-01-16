let oracle = (nqubits, targetState) => {
    let prog = "\t// oracle\n";

    //  Step 1: Apply X gates to flip |0> to |1> for each bit in the target state
    for (i = 0; i < nqubits; i++) {
        if (targetState[i] == '0')
            prog += "\tX[q" + i + "];\n";
    }

    // Step 2: Apply a multi-controlled Z gate (MCZ = H MCX H)
    // prog += "\tH[" + "q" + (nqubits - 1) + "];\n";
    let qubits = "";
    for (i = 0; i < nqubits; i++) {
        qubits += "q" + i + ((i == (nqubits - 1)) ? "" : ", ");
    }
    prog += "\tMCZ[" + qubits + "];\n";
    // prog += "\tMCX[" + qubits + "];\n";
    // prog += "\tH[" + "q" + (nqubits - 1) + "];\n";

    // Step 3: Undo X gates
    for (i = 0; i < nqubits; i++) {
        if (targetState[i] == '0')
            prog += "\tX[q" + i + "];\n";
    }
    return prog;
}

module.exports = {oracle};