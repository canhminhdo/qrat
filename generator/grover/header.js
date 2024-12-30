const { nqubits } = require("./global.js");

let header = () => {
    let prog = "prog GROVER is\n";
    let qubits = "";
    for (i = 0; i < nqubits; i++) {
        qubits += "q" + i + ((i == (nqubits - 1)) ? "" : ", ");
    }
    prog += "var " + qubits + " : qubit;\n";
    return prog;
}

module.exports = {header};