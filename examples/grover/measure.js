const { nqubits } = require("./global.js");

let measure = () => {
    let prog = "\t// measure all qubits\n";
    for (i = 0; i < nqubits; i++) {
        prog += "\tif M[q" + i + "] = 1 then skip; else skip; fi;\n";
    }
    return prog;
}

module.exports = {measure};