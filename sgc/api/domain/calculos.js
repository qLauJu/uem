module.exports = app => {
    return {
        calcularValorPedido: (precoCacamba, qtdeCacambas, qtdeDiasSolicitado, qtdeDiasPadrao) => {
            return ((precoCacamba * qtdeDiasSolicitado) / qtdeDiasPadrao) * qtdeCacambas;
        },

        calcularDataDevolucao: (dataEntrega, qtdeDias) => {
            var dataDevolucao = new Date(dataEntrega);
            dataDevolucao.setDate(dataEntrega.getDate() + parseInt(qtdeDias) + 1);
            return dataDevolucao;
        }
    }
}