let init = (nqubits) => {
    let prog = "\t// initialize superposition qubits\n";
    for (i = 0; i < nqubits; i++) {
        prog += "\tH[q" + i + "];\n";
    }
    return prog;
}

module.exports = {init};