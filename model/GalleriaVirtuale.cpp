#include "GalleriaVirtuale.h"
#include "Controller.h"

// "METODI DI UTILITA'"
void GalleriaVirtuale::setController(Controller *c) { controller = c; }

void GalleriaVirtuale::resetFilters() const {
  searchBox->clear();
  typeBox->setCurrentIndex(0);
  showBox->setCurrentIndex(0);
}

void GalleriaVirtuale::showEmptyList() {
  operaImg.load(":/img/noOpera.jpg");
  operaLabel->setPixmap(operaImg);
  operaImageLayout->update();
  resetLayout(midInfoLayout);
  authorInfoLayout = new QFormLayout;
  operaInfoLayout = new QFormLayout;
  galleryInfoLayout = new QFormLayout;
  authorInfoLayout->addRow("Attenzione ", new QLabel("Nessuna opera in mostra"));
  updateForm();
  mainLayout->update();
}

void GalleriaVirtuale::updateMidLayout(QHBoxLayout* layout, DeepPtr<Opera> &op) {
  resetLayout(layout);
  authorInfoLayout = new QFormLayout;
  operaInfoLayout = new QFormLayout;
  galleryInfoLayout = new QFormLayout;
  inserisciDescrizioneOpera(op);
  insertImg(op->getImgPath());
  operaLabel->setPixmap(operaImg);
  operaImageLayout->update();
  updateForm();
  mainLayout->update();
}

void GalleriaVirtuale::updateEntireView() {
  resetLayout(midInfoLayout);
  listOperaContainer = controller->getOperaContainer();
  changeListener();
}

void GalleriaVirtuale::insertImg(const QString& operaPath){
  operaImg.load(operaPath);
  operaLabel->setPixmap(operaImg);
}

void GalleriaVirtuale::deleteList(const QString & str) const {
  for (int i = 0; i < operaList->count(); ++i) {
    QListWidgetItem* listItem = operaList->item(i);
    if (!listItem->text().toLower().startsWith(str.toLower())) {
      operaList->takeItem(i);
      --i;
    }
  }
}

QFrame* GalleriaVirtuale::createLine() const {
  QFrame* line = new QFrame;
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  return line;
}

void GalleriaVirtuale::resetLayout(QLayout* layout) const{
  while(layout->count() > 0){
    QLayoutItem* item = layout->takeAt(0);
    QWidget* widget = item->widget();
    if (widget) widget->deleteLater();
    else if (item->layout()) resetLayout(item->layout());
    delete item;
  }
}

void GalleriaVirtuale::updateForm() const {
  midInfoLayout->addWidget(createLine());
  midInfoLayout->addLayout(operaInfoLayout);
  midInfoLayout->addLayout(authorInfoLayout);
  midInfoLayout->addLayout(galleryInfoLayout);
  // MAGIA
  midInfoLayout->addStretch();
  midInfoWidget->setLayout(midInfoLayout);
  midLayout->addWidget(midInfoWidget);
}

void GalleriaVirtuale::resetNameList() const {
  if (operaList->count() != 0) { operaList->reset(); operaList->clear(); }
}

QDialog* GalleriaVirtuale::buildDialog() const {
  QDialog* dialog = new QDialog();
  dialog->setWindowTitle("Galleria virtuale");
  dialog->resize(260, 200);
  QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  sizePolicy.setHorizontalStretch(0);
  sizePolicy.setVerticalStretch(0);
  sizePolicy.setHeightForWidth(dialog->sizePolicy().hasHeightForWidth());
  dialog->setSizePolicy(sizePolicy);
  dialog->setMinimumSize(QSize(260, 200));
  dialog->setMaximumSize(QSize(260, 200));
  dialog->setSizeGripEnabled(false);
  dialog->setAttribute(Qt::WA_DeleteOnClose); // Evito memory leak
  dialog->setWindowIcon(QIcon(":/img/appIcona.jpg"));
  return dialog;
}

void GalleriaVirtuale::showInfoMsg() const {
  QDialog* dialog = buildDialog();
  QVBoxLayout* layout = new QVBoxLayout(dialog);
  layout->addWidget(new QLabel("Mostra virtuale di opere d'arte", dialog), 0, Qt::AlignCenter);
  layout->addWidget(new QLabel("Versione: 1.0.0", dialog),0,  Qt::AlignCenter);
  layout->addWidget(new QLabel("Dev: Alessio Berton", dialog), 0, Qt::AlignCenter);
  layout->addWidget(new QLabel("Mail: alessio.berton@studenti.it", dialog), 0, Qt::AlignCenter);
  dialog->exec(); //Blocco il widget
}

void GalleriaVirtuale::showErrorMessage(const QString& message) const {
  QDialog* dialog = buildDialog();
  QVBoxLayout* layout = new QVBoxLayout(dialog);
  layout->addWidget(new QLabel(message, dialog), 0, Qt::AlignCenter);
  dialog->exec(); //Blocco il widget
}

// GUI APPLICAZIONE
GalleriaVirtuale::GalleriaVirtuale(Controller* c, QWidget *parent) : QMainWindow(parent), controller(c) {
  setWindowIcon(QIcon(":/img/appIcona.jpg"));
  setFixedSize(1700, 880);
  appLayout = new QVBoxLayout;
  midLayout = new QVBoxLayout;
  listOperaContainer = controller->getOperaContainer();
  addMenu();
  createTopLayout();
  buildMidLayout();
  createBottomLayout();
  appLayout->addLayout(mainLayout);
  appLayout->setSpacing(0);
  QWidget* mainWidget = new QWidget(this);
  mainWidget->setLayout(appLayout);
  setCentralWidget(mainWidget);
}

void GalleriaVirtuale::addMenu() {
  mainLayout = new QVBoxLayout;
  toolbarLayout = new QHBoxLayout;
  menuBar = new QMenuBar;
  QMenu* file = new QMenu("File", menuBar);
  QMenu* help = new QMenu("Help", menuBar);
  // Menù "File"
  QAction* save= file->addAction("Salva");
  QAction* load= file->addAction("Carica");
  QAction* exit = file->addAction("Chiudi");
  connect(save, SIGNAL(triggered()),controller, SLOT(saveData()));
  connect(load, SIGNAL(triggered()),controller,SLOT(loadData()));
  connect(exit, SIGNAL(triggered()),this,SLOT(close()));
  // Menù "Help"
  QAction* helpButton = help->addAction("Informazioni");
  connect(helpButton, SIGNAL(triggered()), this, SLOT(showInfoMsg()));
  menuBar->addMenu(file);
  menuBar->addMenu(help);
  toolbarLayout->addWidget(menuBar);
  appLayout->addLayout(toolbarLayout);
}

void GalleriaVirtuale::createTopLayout() {
  topLayout = new QHBoxLayout;
  topRightLayout = new QVBoxLayout;
  midInfoLayout = new QHBoxLayout;
  midInfoWidget = new QWidget;
  operaList = new QListWidget;
  operaBox = new QScrollArea;
  operaLabel = new QLabel;
  operaImageLayout = new QHBoxLayout;
  if (!listOperaContainer.isEmpty()) {
      for (int i = 0; i < listOperaContainer.getSize(); i++) {
	if (i == 0) insertImg(listOperaContainer[i]->getImgPath());
	operaList->addItem(new QListWidgetItem(listOperaContainer[i]->getName()));
      }
  }
  operaBox->setWidget(operaList);
  operaBox->setWidgetResizable(true);
  operaBox->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  // C++11 lambda. Non capisco perchè dia warning visto che usare la lambda è addirittura "best practice"
  connect(operaList, &QListWidget::itemSelectionChanged, [this]() {
    if (operaList->count() > 0) {
      int index = operaList->currentRow();
      resetLayout(midInfoLayout);
      buildMidLayout(operaList->item(index)->text());
    }
  });
  operaImageLayout->addWidget(operaLabel);
  topLayout->addLayout(operaImageLayout);
  topRightLayout->addWidget(operaBox);
  topLayout->addLayout(topRightLayout);
  mainLayout->addLayout(topLayout);
}

void GalleriaVirtuale::buildMidLayout(const QString& operaName) {
  authorInfoLayout = new QFormLayout;
  operaInfoLayout = new QFormLayout;
  galleryInfoLayout = new QFormLayout;
  if (!listOperaContainer.isEmpty()) {
    if (operaName == "") { //SITUAZIONE INIZIALE
      inserisciDescrizioneOpera(listOperaContainer[0]);
      updateForm();
      mainLayout->addLayout(midLayout);
    } else { // UPDATE FORM
      const auto listaOpereByName = controller->searchByTpeNameSale(typeBox->currentText(), operaName,
								   showBox->currentText() == "No" ? false : true);
      bool continua = true;
      for (int i = 0; i < listaOpereByName.getSize() && continua ; i++ ) {
	if (listaOpereByName[i]->getName() == operaName) {
	  inserisciDescrizioneOpera(listaOpereByName[i]);
	  insertImg(listaOpereByName[i]->getImgPath());
	  continua = !continua;
	}
      }
      operaImageLayout->update();
      updateForm();
      mainLayout->update();
    }
  } else {
    authorInfoLayout->addRow("Attenzione ", new QLabel("Nessuna opera in mostra"));
    updateForm();
    mainLayout->addLayout(midLayout);
  }
}


void GalleriaVirtuale::changeListener() {
  resetNameList();
  if (!listOperaContainer.isEmpty())
    buildList(searchBox->text(), typeBox->currentText(), showBox->currentText() == "No" ? false : true);
  else {
   showEmptyList();
  }
}

void GalleriaVirtuale::createBottomLayout() {
  bottomLayout= new QVBoxLayout;
  searchBox = new QLineEdit;
  typeBox = new QComboBox;
  showBox = new QComboBox;
  // Search
  QLabel* searchLabel = new QLabel("Cerca");
  searchLabel->setStyleSheet("font-weight: bold; color: black; margin-top: 10px");
  searchBox->setFixedSize(170,25);
  // C++11 lambda. Non capisco perchè dia warning visto che usare la lambda è addirittura "best practice"
  connect(searchBox, &QLineEdit::textChanged, [this]() { changeListener(); });
  // Type
  QLabel* typeLabel = new QLabel("Opera");
  typeLabel->setStyleSheet("font-weight: bold; color: black; margin-top: 10px");
  typeBox->setFixedSize(170, 25);
  typeBox->addItem("Tutte");
  typeBox->addItem("Dipinto");
  typeBox->addItem("Quadro");
  typeBox->addItem("Scultura");
  typeBox->addItem("Mosaico");
  connect(typeBox, &QComboBox::currentTextChanged, [this]() { changeListener(); });
  // Show
  QLabel* showLabel = new QLabel("Solo in vendita");
  showLabel->setStyleSheet("font-weight: bold; color: black; margin-top: 10px");
  showBox->setFixedSize(170, 25);
  showBox->addItem("No");
  showBox->addItem("Si");
  connect(showBox, &QComboBox::currentTextChanged, [this]() { changeListener(); });
  // Reset
  QPushButton* resetButton = new QPushButton("Rimuovi filtri");
  resetButton->setStyleSheet("font-weight: bold; color: black; margin-top: 10px");
  resetButton->setFixedSize(170, 40);
  connect(resetButton, SIGNAL(clicked()), this, SLOT(resetFilters()));
  // DELETE
  QPushButton* deleteButton = new QPushButton("CANCELLA OPERA");
  deleteButton->setStyleSheet("font-weight: bold; color: black; margin-top: 15px");
  deleteButton->setFixedSize(170, 60);
  connect(deleteButton, &QPushButton::clicked, [this]() {
    if (operaList->currentRow() != -1){
      const QString operaName = operaList->currentItem()->text();
      controller->deleteBtnClicked(controller->findOperaGivenName(operaName));
    } else showErrorMessage("Nessuna opera selezionata!");
  });
  bottomLayout->addWidget(searchLabel);
  bottomLayout->addWidget(searchBox);
  bottomLayout->addWidget(typeLabel);
  bottomLayout->addWidget(typeBox);
  bottomLayout->addWidget(showLabel);
  bottomLayout->addWidget(showBox);
  bottomLayout->addWidget(resetButton);
  bottomLayout->addWidget(deleteButton);
  bottomLayout->setContentsMargins(350,0,0,0);
  bottomLayout->addStretch();
  mainLayout->addLayout(bottomLayout);
}

void GalleriaVirtuale::inserisciDescrizioneOpera(DeepPtr<Opera>& singleOpera) const {
  QString format = "dd MMMM yyyy";
  QLocale locale = QLocale(QLocale::Italian);
  QLabel* infoAuthor = new QLabel("Informazioni Autore");
  infoAuthor->setStyleSheet("font-weight: bold; color: black");
  QLabel* infoOpera = new QLabel();
  infoOpera->setStyleSheet("font-weight: bold; color: black");
  QLabel* infoMostra = new QLabel("Informazioni Galleria");
  infoMostra->setStyleSheet("font-weight: bold; color: black");

  operaInfoLayout->addRow(infoOpera);
  authorInfoLayout->setContentsMargins(100,0,0,0);
  authorInfoLayout->addRow(infoAuthor);
  authorInfoLayout->addRow("Nome Autore: ", new QLabel(singleOpera->getAuthor()));
  authorInfoLayout->addRow("Opere esposte dall' autore: ", new QLabel(QString::number(controller->countSameAuthor(singleOpera->getAuthor()))));
  galleryInfoLayout->setContentsMargins(100,0,0,0);
  galleryInfoLayout->addRow(infoMostra);
  galleryInfoLayout->addRow("Data creazione: ", new QLabel(locale.toString(singleOpera->getCreationDate(), format)));
  galleryInfoLayout->addRow("Data prima esposizione: ", new QLabel(locale.toString(singleOpera->getExpositionDate(), format)));
  galleryInfoLayout->addRow("Al momento in vendita in galleria: ", new QLabel(singleOpera->isOnSale() ? "SI" : "NO"));
  const Quadro* quadro = dynamic_cast<Quadro*>(singleOpera.operator->());
  if (quadro) {
    infoOpera->setText("Informazioni Quadro");
    operaInfoLayout->addRow("Nome Quadro: ", new QLabel(quadro->getName()));
    operaInfoLayout->addRow("Altezza in centimetri: ", new QLabel(QString::number(1.0 * quadro->getHeight())));
    operaInfoLayout->addRow("Larghezza in centimetri: ", new QLabel(QString::number(1.0 * quadro->getWidth())));
    operaInfoLayout->addRow("Prezzo in euro: ", new QLabel(QString("%L1").arg(quadro->getPrice())));
    operaInfoLayout->addRow(createLine());
    operaInfoLayout->addRow("Tipo pittura: ", new QLabel(quadro->getDipintoTypeString()));
    operaInfoLayout->addRow("Materiale cornice: ", new QLabel(quadro->getQuadroMaterialString()));
  } else if (dynamic_cast<Scultura*>(singleOpera.operator->())) {
    const Scultura* scultura = static_cast<Scultura*>(singleOpera.operator->());
    if (scultura) {
      infoOpera->setText("Informazioni Scultura");
      operaInfoLayout->addRow("Nome Scultura: ", new QLabel(scultura->getName()));
      operaInfoLayout->addRow("Altezza in metri: ", new QLabel(QString::number(1.0 * scultura->getHeight()/100)));
      operaInfoLayout->addRow("Larghezza in metri: ", new QLabel(QString::number(1.0 * scultura->getWidth()/100)));
      operaInfoLayout->addRow("Prezzo in euro: ", new QLabel(QString("%L1").arg(scultura->getPrice())));
      operaInfoLayout->addRow(createLine());
      operaInfoLayout->addRow("Materiale utilizzato: ", new QLabel(scultura->getSculturaMaterialString()));
    }
  } else if (dynamic_cast<Mosaico*>(singleOpera.operator->())) {
    const Mosaico* mosaico = static_cast<Mosaico*>(singleOpera.operator->());
    if (mosaico) {
      infoOpera->setText("Informazioni Mosaico");
      operaInfoLayout->addRow("Nome mosaico: ", new QLabel(mosaico->getName()));
      operaInfoLayout->addRow("Altezza in centimetri: ", new QLabel(QString::number(1.0 * mosaico->getHeight())));
      operaInfoLayout->addRow("Larghezza in centimetri: ", new QLabel(QString::number(1.0 * mosaico->getWidth())));
      operaInfoLayout->addRow("Prezzo in euro: ", new QLabel(QString("%L1").arg(mosaico->getPrice())));
      operaInfoLayout->addRow(createLine());
      operaInfoLayout->addRow("Materiale utilizzato: ", new QLabel(mosaico->getMosaicoMaterialString()));
    }
  } else if (dynamic_cast<Dipinto*>(singleOpera.operator->())) {
    const Dipinto* dipinto = static_cast<Dipinto*>(singleOpera.operator->());
    if (dipinto) {
      infoOpera->setText("Informazioni Dipinto");
      operaInfoLayout->addRow("Nome dipinto: ", new QLabel(dipinto->getName()));
      operaInfoLayout->addRow("Altezza in centimetri: ", new QLabel(QString::number(1.0 * dipinto->getHeight())));
      operaInfoLayout->addRow("Larghezza in centimetri: ", new QLabel(QString::number(1.0 * dipinto->getWidth())));
      operaInfoLayout->addRow("Prezzo in euro: ", new QLabel(QString("%L1").arg(dipinto->getPrice())));
      operaInfoLayout->addRow(createLine());
      operaInfoLayout->addRow("Tipo pittura: ", new QLabel(dipinto->getDipintoTypeString()));
    }
  }
}

void GalleriaVirtuale::buildList(const QString& nameStr, const QString& showStr, bool typeSale) {
  auto opereToChose = controller->searchByTpeNameSale(showStr, nameStr, showBox->currentText() == "No" ? false : true);
  if (!opereToChose.isEmpty()) {
    showList(typeSale, opereToChose);
    deleteList(nameStr);
  } else {
    showEmptyList();
  }
}

void GalleriaVirtuale::rebuildAfterSearch(DeepPtr<Opera>& op, bool &showFirstImage, bool typeSale) {
  if (typeSale) {
    if (op->isOnSale()) operaList->addItem(new QListWidgetItem(op->getName()));
  } else operaList->addItem(new QListWidgetItem(op->getName()));
  if (showFirstImage) {
    updateMidLayout(midInfoLayout, op);
    showFirstImage = false;
  }
}

void GalleriaVirtuale::showList(bool typeSale, Container<DeepPtr<Opera>> opereToShow) {
  bool showFirstImage = true;
  for (int i = 0; i < opereToShow.getSize(); ++i) rebuildAfterSearch(opereToShow[i], showFirstImage, typeSale);
}
