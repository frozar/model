// Documentation: tutorial from Firecasts
// https://www.youtube.com/watch?v=2Vf1D-rUMwE

// Your web app's Firebase configuration
// For Firebase JS SDK v7.20.0 and later, measurementId is optional
let firebaseConfig = {
    apiKey: "AIzaSyARzybnP5Ko1ZkNaUxf_CXujIcO1wfpj0w",
    authDomain: "shadow-firebase-f4ad5.firebaseapp.com",
    databaseURL: "https://shadow-firebase-f4ad5.firebaseio.com",
    projectId: "shadow-firebase-f4ad5",
    storageBucket: "shadow-firebase-f4ad5.appspot.com",
    messagingSenderId: "1092080899106",
    appId: "1:1092080899106:web:68302bf51bd9a64687a67b",
    measurementId: "G-P7H2ZEP4JN"
};
// Initialize Firebase
firebase.initializeApp(firebaseConfig);
firebase.analytics();
let firestore = firebase.firestore();

const docRef = firestore.doc("samples/sandwichData");
const outputHeader = document.querySelector("#hotDogOutput");
const inputTextField = document.querySelector("#latestHotDogStatus");
const saveButton = document.querySelector("#saveButton");
const loadButton = document.querySelector("#loadButton");

// save to cloud firestore
saveButton.addEventListener("click", function () {
    const textToSave = inputTextField.value;
    console.log("I am going to save " + textToSave + " to Firestore");
    docRef.set({
        hotDogStatus: textToSave
    }).then(function () {
        console.log("Status saved!");
    }).catch(function (error) {
        console.log("Got an error: ", error);
    });
});

// load from cloud firestore: fetch fashion
loadButton.addEventListener("click", function () {
    docRef.get().then(function (doc) {
        if (doc && doc.exists) {
            const myData = doc.data();
            outputHeader.innerText = "Hot dog status: " + myData.hotDogStatus;
        }
    }).catch(function (error) {
        console.log("Got an error: ", error);
    });
});

// load from cloud firestore: real time fashion
getRealtimeUpdates = function () {
    // This object option will launch this function again if only metadata of
    // the snapshot has changed, particularly "hasPendingWrites". If this option
    // is used, this listener will be trigger twice during an update:
    // - when the local database is updated
    // - when the remote database is updated
    // docRef.onSnapshot({includeMetadataChanges: true}, function (doc) {
    docRef.onSnapshot(function (doc) {
        if (doc && doc.exists) {
            const myData = doc.data();
            // Useful to inspect the metadata field of the snapshot
            // to check if the remote document has a pending write on it or not.
            // If it has a pending write, so the current snapshot is coming
            // from the local memory of the client.
            console.log("Check out the document I received ", doc);
            outputHeader.innerText = "Hot dog status: " + myData.hotDogStatus;
        }
    });
};

getRealtimeUpdates();
