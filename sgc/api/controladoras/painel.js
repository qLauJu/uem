module.exports = app => {

    const sequelize = app.db.sequelize;
    const Pedidos = app.db.models.Pedido;

    return {
        getPainelGerente: (req, res) => {
            Pedidos.findAll({
                attributes: [
                    [sequelize.fn('COUNT', sequelize.col('id')), 'nroPedidos'],
                    [sequelize.fn('SUM', sequelize.col('valorTotal')), 'faturamento'],
                ]
            })
                .then(pedidos => {
                    res.json(pedidos[0]);
                })
                .catch(error => {
                    res.status(412).send({
                        mensagem: error.message,
                        sucesso: false
                    });
                });
        },

        getPainelCliente: (req, res) => {
            Pedidos.findAll({
                where: { clienteDocumento: req.user.documento },
                attributes: [
                    [sequelize.fn('COUNT', sequelize.col('id')), 'nroPedidos'],
                    [sequelize.fn('SUM', sequelize.col('valorTotal')), 'faturamento'],
                ]
            })
                .then(pedidos => {
                    res.json(pedidos[0]);
                })
                .catch(error => {
                    res.status(412).send({
                        mensagem: error.message,
                        sucesso: false
                    });
                });
        }
    }
}
