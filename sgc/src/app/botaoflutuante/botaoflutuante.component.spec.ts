import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { BotaoflutuanteComponent } from './botaoflutuante.component';

describe('BotaoflutuanteComponent', () => {
  let component: BotaoflutuanteComponent;
  let fixture: ComponentFixture<BotaoflutuanteComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ BotaoflutuanteComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(BotaoflutuanteComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
