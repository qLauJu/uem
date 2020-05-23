import { Injectable } from '@angular/core';
import { BaseService } from '../base/base.service';
import { Headers, Http } from '@angular/http';
import { Pedido } from '../../models/pedido';
import { CalcularPedidoResponse } from '../response-models/calcular-pedido-response';
import { DefaultResponse } from '../response-models/default-response';

@Injectable()
export class PedidosService extends BaseService {

  constructor(private http: Http) {
    super(http);
  }

  criarPedido(pedido: Pedido): Promise<Pedido> {
    return this.http.post(this.baseUrl + '/pedidos', pedido, {headers: this.headers}).toPromise()
      .then(response => response.json() as Pedido)
      .catch(this.handleError);
  }

  editarPedido(pedido: Pedido): Promise<DefaultResponse> {
    return this.http.put(this.baseUrl + `/pedidos/${pedido.id}`, pedido, {headers: this.headers}).toPromise()
      .then(response => response.json() as DefaultResponse)
      .catch(error => {
        console.log(error);
        return this.handleError(error);
      });
  }

  getPedidos(): Promise<Pedido[]> {
    return this.http.get(this.baseUrl + '/pedidos', {headers: this.headers}).toPromise()
      .then(response => response.json() as Pedido[])
      .catch(this.handleError);
  }

  calcularValor(pedido: Pedido): Promise<CalcularPedidoResponse> {
    return this.http.post(this.baseUrl + '/pedidos/calcular', pedido, {headers: this.headers}).toPromise()
      .then(response => response.json() as CalcularPedidoResponse)
      .catch(this.handleError);
  }

  remover(id: number): Promise<any> {
    return this.http.delete(this.baseUrl + `/pedidos/${id}`, {headers: this.headers}).toPromise();
  }

  getStatuses(): Promise<[string]> {
    return this.http.get(this.baseUrl + '/pedidos/status', {headers: this.headers}).toPromise()
      .then(response => response.json() as string[])
      .catch(this.handleError);
  }

}
