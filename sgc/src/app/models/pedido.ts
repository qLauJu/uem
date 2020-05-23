export class Pedido {
    id: number;
    endereco: string;
    tamanhoCacamba: string;
    descricaoCacamba: string;
    qtdeCacambas: number;
    dataEntrega: Date;
    qtdeDias: number;
    dataDevolucao: Date;
    valorTotal: number;
    clienteDocumento: string;
    CacambaPrecoId: number;
    status: string;
    observacoes: string;
}
