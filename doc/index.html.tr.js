const Dict = {
    "写作素材管理器": "Writing Materials Manager",
    "架构图": "Architecture Diagram",
}

const LanguageSelector = document.getElementById('lang-selector');
LanguageSelector.addEventListener('change', (event) => {

})

const ElementsTranslate = document.querySelectorAll('.tr')
ElementsTranslate.forEach(e => {
    const TranslatedText = Dict[e.innerHTML]
    if (TranslatedText) e.innerHTML = TranslatedText
})
