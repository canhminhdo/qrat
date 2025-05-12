let pcheck = (nqubits) => {
    let prog = "\n";
    prog += "pcheck in GROVER with 'P=? [F \"target\"]' .";
    return prog;
}

module.exports = {pcheck};