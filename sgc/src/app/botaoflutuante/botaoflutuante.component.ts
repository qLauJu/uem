import { Component, OnInit, HostListener } from '@angular/core';

@Component({
  selector: 'app-botaoflutuante',
  templateUrl: './botaoflutuante.component.html',
  styleUrls: ['./botaoflutuante.component.css']
})
export class BotaoflutuanteComponent implements OnInit {

  private tipoUsuario: string;

  modalAparecendo = false;
  mostrarCriarPedido = false;
  mostrarCriarCliente = false;
  mostrarCriarCacamba = false;
  mostrarMenuOpcoes = false;

  constructor() { }

  @HostListener('document:keydown', ['$event'])
  handleKeyEvent(event: KeyboardEvent): void {
    if (event.keyCode === 27) {
      this.esconderModal();
    }
  }

  ngOnInit() {
    this.tipoUsuario = localStorage.getItem('usuarioTipo');
  }

  mostrarModal(): void {
   this.esconderTodoConteudo();
   this.modalAparecendo = true;
   this.mostrarConteudoDeAcordoComUsuario(); 
  }

  esconderModal(): void {
    this.modalAparecendo = false;
    this.esconderTodoConteudo();
  }

  mostrarFormularioPedido(): void {
    this.mostrarMenuOpcoes = false;
    this.mostrarCriarCacamba = false;
    this.mostrarCriarPedido = true;
    this.mostrarCriarCliente = false;
  }

  mostrarFormularioCliente(): void {
    this.mostrarMenuOpcoes = false;
    this.mostrarCriarCacamba = false;
    this.mostrarCriarPedido = false;
    this.mostrarCriarCliente = true;
  }

  mostrarFormularioCacamba(): void {
    this.mostrarMenuOpcoes = false;
    this.mostrarCriarCacamba = true;
    this.mostrarCriarPedido = false;
    this.mostrarCriarCliente = false;
  }

  private esconderTodoConteudo(): void {
    this.mostrarCriarPedido = false;
    this.mostrarCriarCacamba = false;
    this.mostrarMenuOpcoes = false;
    this.mostrarCriarCliente = false;
  }

  private mostrarConteudoDeAcordoComUsuario(): void {
    if (this.tipoUsuario === 'GERENTE') {
      this.mostrarMenuOpcoes = true;
    } else {
      this.mostrarCriarPedido = true;
    }
  }

}
