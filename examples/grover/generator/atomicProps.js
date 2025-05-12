let atomicProps = (nqubits, targetState) => {
    prog = "prop\n";
    let props = "";
    for (i = 0; i < nqubits; i++) {
        if (targetState[i] == "0") {
            props += "\ttarget" + i + " := P(q" + i + ", |0>);\n";
        } else {
            props += "\ttarget" + i + " := P(q" + i + ", |1>);\n";
        }
    }
    prog += props;
    return prog;
}

module.exports = {atomicProps};