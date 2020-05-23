module.exports = (sequelize, DataType) => {
    const Pedido = sequelize.define("Pedido", {
        id: {
            type: DataType.INTEGER,
            primaryKey: true,
            autoIncrement: true
        },
        endereco: {
            type: DataType.STRING,
            allowNull: false,
            validate: {
                notEmpty: true
            }
        },
        qtdeCacambas: {
            type: DataType.INTEGER,
            allowNull: false
        },
        dataEntrega: {
            type: DataType.DATE
        },
        qtdeDias: {
            type: DataType.INTEGER,
            allowNull: false
        },
        dataDevolucao: {
            type: DataType.DATE
        },
        valorTotal: {
            type: DataType.DOUBLE,
            allowNull: false
        },
        status: {
            type: DataType.STRING,
            allowNull: false,
            defaultValue: "Aguardando entrega",
            validate: {
                notEmpty: true
            }
        },
        observacoes: {
            type: DataType.STRING
        },
        clienteDocumento: {
            type: DataType.STRING,
            allowNull: false,
            validate: {
                notEmpty: true
            }
        }
    });

    Pedido.associate = models => {
         Pedido.belongsTo(models.CacambaPreco);
    }

    return Pedido;
}