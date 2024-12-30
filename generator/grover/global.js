targetState = "01101011010101101101";
nqubits = targetState.length;
nIters = Math.floor(Math.PI / 4 * Math.sqrt(Math.pow(2, nqubits)));

module.exports = {nqubits, targetState, nIters};