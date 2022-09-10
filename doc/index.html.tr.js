const Dict = {
    "写作素材管理器": "Writing Materials Manager",
    "架构图": "Architecture Diagram",
}

const LanguageSelector = document.getElementById('lang-selector');
var CurrentLanguage = 'zh-CN'
LanguageSelector.addEventListener('change', (event) => {
    console.log(event.target.value)
})

const ElementsToTranslate = document.querySelectorAll('.tr')
ElementsToTranslate.forEach(e => {
    const TranslatedText = Dict[e.innerHTML]
    if (TranslatedText) e.innerHTML = TranslatedText
})
