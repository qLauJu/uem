import { Component, OnInit, HostListener, Output } from '@angular/core';
import { AuthService } from '../services/auth/auth.service';
import { PedidosService } from '../services/pedidos/pedidos.service';
import { Pedido } from '../models/pedido';

@Component({
  selector: 'app-pedidos',
  templateUrl: './pedidos.component.html',
  styleUrls: ['./pedidos.component.css'],
  providers: [PedidosService]
})
export class PedidosComponent implements OnInit {

  pedidoId: number;
  mostraIcones = false;
  private tipoUsuario: string;
  private isGerente = false;

  @Output()
  selectedPedido: Pedido;

  pedidos: Pedido[];
  statuses: string[] = [];

  constructor(private authService: AuthService, private pedidosService: PedidosService) { }

  @HostListener('document:keydown', ['$event'])
  handleKeyEvent(event: KeyboardEvent): void {
    if (event.keyCode === 27) {
      this.esconderModal();
    }
  }

  ngOnInit() {
    this.tipoUsuario = localStorage.getItem('usuarioTipo');
    this.isGerente = this.tipoUsuario === 'GERENTE';
    this.getPedidos();
    this.getStatuses();
  }

  private getStatuses(): void {
    this.pedidosService.getStatuses()
      .then(statuses => {
        this.statuses = statuses;
      })
      .catch(error => console.log(error));
  }

  private getPedidos(): void {
    this.pedidosService.getPedidos()
      .then(pedidos => this.pedidos = pedidos)
      .catch(error => console.log(error));
  }

  editar(pedido: Pedido): void {
    this.selectedPedido = pedido;
  }

  excluir(id: number): void {
    this.pedidosService.remover(id)
      .then(response => this.getPedidos())
      .catch(error => console.log(error));
  }

  esconderModal(): void {
    this.selectedPedido = null;
  }

  mostrarIcones(idPedido: number): void {
    this.pedidoId = idPedido;
    this.mostraIcones = true;
  }

  esconderIcones(): void {
    this.mostraIcones = false;
  }

  alterarStatus(pedido: Pedido): void {
    if (this.isGerente) {
      this.pedidosService.editarPedido(pedido);
    }
  }
}
