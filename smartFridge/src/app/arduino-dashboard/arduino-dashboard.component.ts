import { Component, OnInit } from '@angular/core';
import { IArduinoData } from './arduinoData';
import { HttpClient } from '@angular/common/http';

@Component({
    selector: 'app-arduino-view',
    templateUrl: 'arduino-dashboard.component.html'
})
export class ArduinoDashboardComponent implements OnInit {
    public arduinoData: IArduinoData;
    constructor(private httpClient: HttpClient) { }

    ngOnInit() {
        this.httpClient.get('http://127.0.0.1:5000/').subscribe((res: any) => {
            this.arduinoData.humidity = res.humidity;
            this.arduinoData.temperature = res.temperature;
            this.arduinoData.door = res.door;
        });
    }
}
