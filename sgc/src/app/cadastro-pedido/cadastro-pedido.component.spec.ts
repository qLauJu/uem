import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { CadastroPedidoComponent } from './cadastro-pedido.component';

describe('CadastroPedidoComponent', () => {
  let component: CadastroPedidoComponent;
  let fixture: ComponentFixture<CadastroPedidoComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ CadastroPedidoComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(CadastroPedidoComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should be created', () => {
    expect(component).toBeTruthy();
  });
});
