import { TestBed, inject } from '@angular/core/testing';

import { PainelService } from './painel.service';

describe('PainelService', () => {
  beforeEach(() => {
    TestBed.configureTestingModule({
      providers: [PainelService]
    });
  });

  it('should be created', inject([PainelService], (service: PainelService) => {
    expect(service).toBeTruthy();
  }));
});
