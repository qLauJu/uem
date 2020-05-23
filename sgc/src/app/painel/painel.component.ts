import { Component, OnInit } from '@angular/core';
import { PainelService } from '../services/painel/painel.service';
import { PainelResponse } from '../services/response-models/painel-response';
import { PedidosService } from '../services/pedidos/pedidos.service';
import { Pedido } from '../models/pedido';

@Component({
  selector: 'app-painel',
  templateUrl: './painel.component.html',
  styleUrls: ['./painel.component.css'],
  providers: [PainelService, PedidosService]
})
export class PainelComponent implements OnInit {

  painel = new PainelResponse();
  pedidos: Pedido[] = [];

  constructor(private painelService: PainelService, private pedidosService: PedidosService) { }

  ngOnInit() {
    this.getPainel();
    this.getPedidos();
  }

  private getPainel(): void {
    this.painelService.getPainel()
      .then(response => this.painel = response)
      .catch(error => console.log(error));
  }

  private getPedidos(): void {
    this.pedidosService.getPedidos()
      .then(pedidos => this.pedidos = pedidos)
      .catch(error => console.log(error));
  }

}
