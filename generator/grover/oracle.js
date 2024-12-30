const { nqubits, targetState } = require("./global.js");

let oracle = () => {
    let prog = "\t// oracle\n";

    //  Step 1: Apply X gates to flip |0> to |1> for each bit in the target state
    for (i = 0; i < nqubits; i++) {
        if (targetState[i] == '0')
            prog += "\tq" + i + " := X[q" + i + "];\n";
    }

    // Step 2: Apply a multi-controlled Z gate (MCZ)
    prog += "\tq" + (nqubits - 1) + " := H[" + "q" + (nqubits - 1) + "];\n";
    let qubits = "";
    for (i = 0; i < nqubits; i++) {
        qubits += "q" + i + ((i == (nqubits - 1)) ? "" : ", ");
    }
    prog += "\t" + qubits + " := MCX[" + qubits + "];\n";
    prog += "\tq" + (nqubits - 1) + " := H[" + "q" + (nqubits - 1) + "];\n";

    // Step 3: Undo X gates
    for (i = 0; i < nqubits; i++) {
        if (targetState[i] == '0')
            prog += "\tq" + i + " := X[q" + i + "];\n";
    }
    return prog;
}

module.exports = {oracle};