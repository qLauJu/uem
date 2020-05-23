import { Component, OnInit, Output } from '@angular/core';
import { Cacamba } from '../models/cacamba';
import { CacambasService } from '../services/cacambas/cacambas.service';

@Component({
  selector: 'app-cacambas',
  templateUrl: './cacambas.component.html',
  styleUrls: ['./cacambas.component.css'],
  providers: [CacambasService]
})
export class CacambasComponent implements OnInit {

  @Output()
  cacambaSelecionada: Cacamba;

  cacambas: Cacamba[];

  mostraIcones = false;
  cacambaId: number;

  constructor(private cacambasService: CacambasService) { }

  ngOnInit() {
    this.getCacambas();
  }

  editar(cacamba: Cacamba): void {
    this.cacambaSelecionada = cacamba;
  }

  private getCacambas(): void {
    this.cacambasService.getCacambas()
      .then(cacambas => this.cacambas = cacambas)
      .catch(error => console.log(error));
  }

  excluir(id: number): void {
    this.cacambasService.removerCacamba(id)
      .then(response => {
        this.getCacambas();
      })
      .catch(error => console.log(error));
  }

  mostrarIcones(idCacamba: number) {
    this.cacambaId = idCacamba;
    this.mostraIcones = true;
  }

  esconderIcones(): void {
    this.mostraIcones = false;
  }

  esconderModal(): void {
    this.cacambaSelecionada = null;
  }

}
