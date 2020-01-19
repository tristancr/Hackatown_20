import { Component, OnInit, Input } from '@angular/core';
import { Recipe } from './recipe';
import { HttpClient } from '@angular/common/http';
import { Product } from './product';
const URLFIRSTPART = 'https://api.edamam.com/search?q=';
const URLSECONDPART = '&app_id=a75732ea&app_key=a7f69a13f95037e6c8cbe8840b5d7a99';
@Component({
  selector: 'app-view',
  templateUrl: './view.component.html',
  styleUrls: ['./view.component.css']
})
export class ViewComponent implements OnInit {
  private productArray: Product[];
  @Input() newProductName: string;
  @Input() newProductExpirationDate: Date;
  private recipeArray: Recipe[];
  private viewProducts: number;
  private viewRecipe: number;
  protected stringProducts: string[];
  protected stringRecipe: string[];
  constructor(private http: HttpClient) {
    this.recipeArray = new Array();
    this.productArray = new Array();
    this.viewProducts = 0;
    this.viewRecipe = 0;
    this.stringProducts = new Array();
    this.stringProducts.push('hide products');
    this.stringProducts.push('see products');
    this.stringRecipe = new Array();
    this.stringRecipe.push('hide recipes');
    this.stringRecipe.push('see recipes');
  }
  ngOnInit() {
  }

  addProduct() {
    this.productArray.push({
      name: this.newProductName,
      expirationDate: this.newProductExpirationDate,
    });
    this.newProductName = '';
  }

  changeProduct() {
    this.viewProducts = this.viewProducts === 0 ? 1 : 0;
  }

  changeRecipe() {
    this.viewRecipe = this.viewRecipe === 0 ? 1 : 0;
    this.findRecipe();
  }

  productsToString(): string {
    let stringProducts = this.productArray[0].name;
    for (let index = 1; index < this.productArray.length; index++) {
      stringProducts += '+' + this.productArray[index];
    }
    return stringProducts;
  }
  findRecipe() {
    // tslint:disable-next-line:max-line-length
    this.http.get(URLFIRSTPART + this.productsToString() + URLSECONDPART)
      .subscribe((response: any) => {
        console.log(response);
        this.recipeArray = [];
        for (const currentRecipe of response.hits) {
          this.recipeArray.push( {
            nom: currentRecipe.recipe.label,
            image: currentRecipe.recipe.image,
            url: currentRecipe.recipe.url
          });
        }
        // response.hits[0]
        console.log(this.recipeArray);
      });
  }
}
