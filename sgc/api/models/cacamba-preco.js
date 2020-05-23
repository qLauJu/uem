module.exports = (sequelize, DataType) => {
    const CacambaPreco = sequelize.define("CacambaPreco", {
        id: {
            type: DataType.INTEGER,
            primaryKey: true,
            autoIncrement: true
        },
        valor: {
            type: DataType.DOUBLE,
            allowNull: false
        },
        descricao: {
            type: DataType.STRING
        }
    });
  
    CacambaPreco.associate = models => {
        CacambaPreco.belongsTo(models.Cacamba);
    }

    return CacambaPreco;
}
