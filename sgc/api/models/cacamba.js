module.exports = (sequelize, DataType) => {
    const Cacamba = sequelize.define("Cacamba", {
        id: {
            type: DataType.INTEGER,
            primaryKey: true,
            autoIncrement: true
        },
        tamanho: {
            type: DataType.STRING,
            allowNull: false,
            unique: true,
            validate: {
                notEmpty: true
            }
        },
        quantidade: {
            type: DataType.INTEGER,
            allowNull: false
        },
    });
    
    Cacamba.associate = models => {}
    
    return Cacamba;
}
