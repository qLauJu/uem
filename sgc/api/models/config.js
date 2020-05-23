module.exports = (sequelize, DataType) => {
    const Config = sequelize.define('Config', {
        userId: {
            type: DataType.INTEGER,
            primaryKey: true
        },
        qtdePadraoDeDias: {
            type: DataType.INTEGER,
            allowNull: false
        },
        termosDeUso: {
            type: DataType.TEXT
        },
        termosDeLocacao: {
            type: DataType.TEXT
        },
        habilitarNotificacao: {
            type: DataType.BOOLEAN,
            allowNull: false,
            default: true,
        }
    });

    Config.associate = models => {}

    return Config;
};
