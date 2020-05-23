import bcrypt from 'bcrypt';

module.exports = (sequelize, DataType) => {
    const Usuario = sequelize.define('Usuario', {
        id: {
            type: DataType.INTEGER,
            primaryKey: true,
            autoIncrement: true
        },
        nome: {
            type: DataType.STRING,
            allowNull: true
        },
        tipo: {
            type: DataType.STRING,
            allowNull: false,
            defaultValue: "CLIENTE",
            validate: {
                notEmpty: true
            }
        },
        documento: {
            type: DataType.STRING,
            allowNull: false,
            unique: true,
            validate: {
                notEmpty: true
            }
        },
        senha: {
            type: DataType.STRING,
            allowNull: false,
            validate: {
                notEmpty: true
            }
        },
        email: {
            type: DataType.STRING,
            allowNull: true
        }
    }, {
        hooks: {
            beforeCreate: usuario => {
                const salt = bcrypt.genSaltSync();
                usuario.senha = bcrypt.hashSync(usuario.senha, salt);
            }
        }
    });

    Usuario.associate = (models) => {};

    Usuario.isPassword = (encodedPassword, password) => {
        console.log(encodedPassword, password);
        return bcrypt.compareSync(password, encodedPassword);
    };

    return Usuario;
}