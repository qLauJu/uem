import { Component, OnInit, Input } from '@angular/core';
import { Cacamba } from '../models/cacamba';
import { CacambasService } from '../services/cacambas/cacambas.service';


@Component({
  selector: 'app-cadastro-cacamba',
  templateUrl: './cadastro-cacamba.component.html',
  styleUrls: ['./cadastro-cacamba.component.css'],
  providers: [CacambasService]
})
export class CadastroCacambaComponent implements OnInit {

  @Input()
  cacamba: Cacamba;

  @Input()
  estaEditando = false;

  private cacambas: Cacamba[];

  estaCarregando = false;
  cacambasTamanhos: string[] = [];
  cacambasDescricoes: string[] = [];
  quantidades: number[] = [];

  constructor(private cacambasService: CacambasService) {
    if (this.estaEditando) {
      this.setupLayoutForEditing();
    } else {
      this.setupLayoutForInserting();
    }
  }

  private setupLayoutForEditing(): void { }

  private setupLayoutForInserting(): void {
    this.cacamba = new Cacamba();
  }

  cadastrar(): void {
    this.estaCarregando = true;
    this.cacambasService.criarCacamba(this.cacamba)
      .then(cacamba => {
        this.estaCarregando = false;
        console.log(cacamba);
      })
      .catch(error => {
        this.estaCarregando = false;
        console.log(error);
      });
  }

  editar(): void {
    this.estaCarregando = true;
    const cacamba = Object.assign({}, this.cacamba); // cloning the object
    cacamba.tamanho = undefined;
    this.cacambasService.editarCacamba(cacamba)
      .then(response => {
        this.estaCarregando = false;
        console.log(response);
      })
      .catch(error => {
        this.estaCarregando = false;
        console.log(error);
      });
  }

  tamanhoChanged(): void {
    this.quantidades = this.cacambas.filter(cacamba => this.cacamba.tamanho === cacamba.tamanho).map(cacamba => cacamba.quantidade);
    console.log(this.quantidades);
    if (this.quantidades.length == 1) {
      this.cacamba.quantidade = this.quantidades[0];
    } else {
      this.cacamba.quantidade = 0;
    }
  }

  ngOnInit() {
    this.getCacambas();
  }

  private getCacambas(): void {
    this.cacambasService.getCacambas()
      .then(cacambas => {
        this.cacambas = cacambas;
        this.cacambasDescricoes = this.uniq(cacambas.map(cacamba => cacamba.descricao));
        this.cacambasTamanhos = this.uniq(cacambas.map(cacamba => cacamba.tamanho));
      })
      .catch(error => console.log(error));
  }

  // Função para remover elementos duplicados do array.
  private uniq(input: string[]): string[] {
    const seen = {};
    const out = [];
    const len = input.length;
    let j = 0;
    for (let i = 0; i < len; i++) {
      const item = input[i];
      if (seen[item] !== 1) {
        seen[item] = 1;
        out[j++] = item;
      }
    }
    return out;
  }

}
