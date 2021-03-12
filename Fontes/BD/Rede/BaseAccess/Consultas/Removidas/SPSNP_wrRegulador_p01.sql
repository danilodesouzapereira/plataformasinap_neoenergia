    INSERT INTO
      Regulador ( intTipoLigacaoId, fltSnom, bolBypass, fltFaixaVar, intNumPasso, bolSentidoReversivel, intSentidoBarRefId, bolAjusteAuto, intCfgFixPassoSel, intCfgAutBarRefId, fltCfgAutVpu, fltZ0R, fltZ0X, fltZ1R, fltZ1X, intEstudoId, intReguladorId )
    VALUES
      ( [p_TipoLigacaoId], [p_Snom], [p_Bypass], [p_FaixaVar], [p_NumPasso], [p_SentidoReversivel], IIF([p_SentidoBarRefId] <> -1, [p_SentidoBarRefId], NULL), [p_AjusteAuto], [p_CfgFixPassoSel], IIF([p_CfgAutBarRefId] <> -1, [p_CfgAutBarRefId], NULL), [p_CfgAutVpu], [p_Z0R], [p_Z0X], [p_Z1R], [p_Z1X], [p_EstudoId], [p_ReguladorId] );
