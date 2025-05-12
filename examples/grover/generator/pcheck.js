let pcheck = (nqubits) => {
    let prog = "\n";
    let list = [];
    for (i = 0; i < nqubits; i++) {
        list.push('"target' + i + '"');
    }
    prog += "pcheck in GROVER with 'P=? [F (" + list.join(" & ") + ")]' .";
    return prog;
}

module.exports = {pcheck};