module.exports = {
    database: 'sgc_test',
    username: '',
    password: '',
    params: {
        dialect: 'sqlite',
        storage: 'sgc.sqlite',
        logging: false,
        define: {
            underscored: false
        }
    },
    jwtSecret: "SGC_TEST",
    jwtSession: {
        session: false
    }
};
