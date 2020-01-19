import { Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root'
})
export class ProductService {
  name: string;
  expirationDate: Date;
  constructor(name: string, expirationDate: Date) {
    this.name = name;
    this.expirationDate = expirationDate;
  }
}
