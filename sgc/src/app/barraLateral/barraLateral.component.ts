import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'app-barraLateral',
  templateUrl: './barraLateral.component.html',
  styleUrls: ['./barraLateral.component.css']
})
export class BarraLateralComponent implements OnInit {

  usuarioGerente = localStorage.getItem('usuarioTipo') === 'GERENTE';

  constructor() { }

  ngOnInit() {
  }

}
