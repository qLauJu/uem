import { Component, OnInit, Input } from '@angular/core';
import { PedidosService } from '../services/pedidos/pedidos.service';
import { CacambasService } from '../services/cacambas/cacambas.service';
import { Pedido } from '../models/pedido';
import { Cacamba } from '../models/cacamba';
import { FormBuilder } from '@angular/forms';
import { UsuariosService } from '../services/usuarios/usuarios.service';

@Component({
  selector: 'app-cadastro-pedido',
  templateUrl: './cadastro-pedido.component.html',
  styleUrls: ['./cadastro-pedido.component.css'],
  providers: [PedidosService, CacambasService, UsuariosService]
})
export class CadastroPedidoComponent implements OnInit {
  tipoUsuario: string;
  mostrarCampoCliente: Boolean = false;
  modalAberto: Boolean = false;

  @Input()
  isEditing = false;
  @Input()
  pedido: Pedido;

  private cacambas: Cacamba[];

  documentosClientes: string[] = [];
  cacambasTamanhos: string[] = [];
  cacambasDescricoes: string[] = [];

  valorTotal = 0;
  dataEntrega = new Date();
  dataDevolucao = new Date();

  errorMessage: string;
  isLoading = false;

  ngOnInit() {
    this.tipoUsuario = localStorage.getItem('usuarioTipo');
    this.mostrarParaUsuarioGerente();
    this.getCacambas();

    if (this.tipoUsuario === 'GERENTE') {
      this.getDocumentos();
    }
  }

  abrirModal(): void {
    this.modalAberto = true;
  }

  constructor(private fb: FormBuilder,
    private pedidosService: PedidosService,
    private cacambasService: CacambasService,
    private usuariosService: UsuariosService) {

    if (this.isEditing) {
      this.setupLayoutForEditing();
    } else {
      this.setupLayoutForInserting();
    }
  }

  private setupLayoutForEditing(): void {
    this.dataEntrega = this.pedido.dataEntrega;
    this.dataDevolucao = this.pedido.dataDevolucao;
    this.valorTotal = this.pedido.valorTotal;
  }

  private setupLayoutForInserting(): void {
    this.pedido = new Pedido();
  }

  cadastrar(): void {
    this.isLoading = true;

    this.pedido.CacambaPrecoId = this.getCacambaId();
    this.pedido.dataEntrega = this.dataEntrega;

    this.pedidosService.criarPedido(this.pedido)
      .then(response => {
        this.isLoading = false;
        this.limparCampos();
      })
      .catch(error => {
        this.isLoading = false;
        this.limparCampos();
        console.log(error);
      });

  }

  editar(): void {
    this.isLoading = true;
    this.pedido.CacambaPrecoId = this.getCacambaId();

    console.log(this.pedido);
    this.pedidosService.editarPedido(this.pedido)
      .then(response => {
        this.isLoading = false;
        // TODO: fechar modal
      })
      .catch(error => {
        console.log(error);
      });
  }

  private getDocumentos(): void {
    this.usuariosService.getDocumentos()
      .then(documentos => this.documentosClientes = documentos)
      .catch(error => console.log(error));
  }

  private getCacambas(): void {
    this.cacambasService.getCacambas()
      .then(cacambas => {
        this.cacambas = cacambas;
        this.cacambasTamanhos = this.uniq(cacambas.map(cacamba => cacamba.tamanho));
        this.cacambasDescricoes = this.uniq(cacambas.map(cacamba => cacamba.descricao));
      })
      .catch(error => console.log(error));
  }

  private getCacambaId(): number {
    const cacambas = this.cacambas.filter(cacamba => cacamba.descricao === this.pedido.descricaoCacamba)
      .filter(cacamba => cacamba.tamanho === this.pedido.tamanhoCacamba);
    if (cacambas.length > 0) {
      return cacambas[0].id;
    }
    return 0;
  }

  private limparCampos(): void {
    this.pedido = new Pedido();
    this.valorTotal = 0;
    this.dataDevolucao = new Date();
  }

  mostrarParaUsuarioGerente(): void {
    this.mostrarCampoCliente = (this.tipoUsuario === 'GERENTE');
  }

  tamanhoCacambaChanged(): void {
    const cacambas = this.cacambas.filter(cacamba => cacamba.tamanho === this.pedido.tamanhoCacamba)
      .map(cacamba => cacamba.descricao);
    this.cacambasDescricoes = this.uniq(cacambas);
    this.formChanged();
  }

  formChanged(): void {
    if (this.pedido.tamanhoCacamba !== undefined &&
      this.pedido.descricaoCacamba !== undefined &&
      this.dataEntrega !== undefined &&
      this.pedido.qtdeCacambas !== undefined &&
      this.pedido.qtdeDias !== undefined) {
      this.pedido.CacambaPrecoId = this.getCacambaId();
      this.pedido.dataEntrega = this.dataEntrega;
      this.pedidosService.calcularValor(this.pedido)
        .then(response => {
          console.log(response);
          this.valorTotal = response.valor;
          this.dataDevolucao = response.dataDevolucao;
        })
        .catch(error => console.log(error));
    } else {
      console.log('Necessita mais informações.');
    }
  }

  // Funcão para remover elementos duplicados do array.
  private uniq(entrada: string[]): string[] {
    const seen = {};
    const saida = [];
    const tamanho = entrada.length;
    let j = 0;
    for (let i = 0; i < tamanho; i++) {
      const item = entrada[i];
      if (seen[item] !== 1) {
        seen[item] = 1;
        saida[j++] = item;
      }
    }
    return saida;
  }

}
