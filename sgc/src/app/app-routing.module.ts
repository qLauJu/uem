import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { BarraLateralComponent } from './barraLateral/barraLateral.component';
import { BarraNavegacaoComponent } from './barraNavegacao/barraNavegacao.component';
import { LoginComponent } from './login/login.component';
import { ClientesComponent } from './clientes/clientes.component';
import { PedidosComponent } from './pedidos/pedidos.component';
import { PainelComponent } from './painel/painel.component';
import { CacambasComponent } from './cacambas/cacambas.component';
import { CriarContaComponent} from './criar-conta/criar-conta.component'

const routes: Routes = [
  { path: '', redirectTo: '/login', pathMatch: 'full' },
  {
    path: 'painel',
    component: BarraNavegacaoComponent,
    children: [
      {
        path: '',
        component: PainelComponent
      },
      {
        path: 'clientes',
        component: ClientesComponent
      },
      {
        path: 'pedidos',
        component: PedidosComponent
      },
      {
        path: 'cacambas',
        component: CacambasComponent
      }
    ]
  },
  { path: 'registrar', component: CriarContaComponent },
  { path: 'login', component: LoginComponent }

];

@NgModule({
    imports: [RouterModule.forRoot(routes)],
    exports: [RouterModule],
    providers: []
})
export class AppRoutingModule { }
